# KL_I2C — 通用型 I2C 总线控制器

移植教学视频：https://www.bilibili.com/video/BV1Mebi6oEKB

一个**与平台无关**的 I2C 主机驱动库，实现了**完全无阻塞（全异步）**的多任务 I2C 通信：

- 任务队列 + 总线状态机，多条总线互不干扰；
- 注册任务后立刻返回，传输在后台进行，完成后通过**回调函数**通知你；
- 发送/接收都支持 **中断** 和 **DMA** 两种方式，按需配置；
- 天然适合 RTOS / 裸机主循环 / 中断频繁的系统。

> **重要：本库不是 STM32 专属！** 核心文件（`KL_I2C.h` / `KL_I2C.c`）**只依赖 C99 环境**（仅用到 `stdint.h`、位域、指定初始化器，外加 `__weak` 这一个主流编译器都支持的扩展）。
> 仓库里的 `KL_I2C_Init.*`、`KL_I2C_conf.*` 只是**以 STM32 + CubeMX 为例**的对接示例，照葫芦画瓢即可移植到任何带 I2C 外设的平台（STC、NXP、GD32、AVR）。

## 移植条件

1. 平台支持使用C语言编程
2. 芯片上有硬件I2C外设，且此外设可以产生“传输完成中断”

## 目录结构

```
KL_I2C/
├── Inc/
│   ├── KL_I2C.h          # 【核心】库接口与数据结构，纯 C99，不用改
│   ├── KL_I2C_conf.h     # 【Conf 配置】平台头文件 / 总线使能 / 队列长度 / DMA 开关
│   └── KL_I2C_Init.h     # 【Init 声明】句柄宏定义、收发函数声明（CubeMX 风格命名）
├── Src/
│   ├── KL_I2C.c          # 【核心】任务队列 + 状态机实现，纯 C99，不用改
│   └── KL_I2C_Init.c     # 【Init 实现】硬件绑定、收发函数、中断回调转发（按平台改这里）
└── LICENSE               # MIT License
```

**一句话：核心（`KL_I2C.h/.c`）一个字都不用动，移植平台只改 `Init` 和 `Conf` 两部分。**

## 库的工作流程（先理解再动手）

```
你的业务代码
    │  KL_I2C_RegisterTask(&KL_I2C1_Handle, MSG, Addr, buff, Len, 回调, NULL)
    ▼
任务队列（环形缓冲，先进先出）＋ 状态机（KL_I2C.c 内部，自动排队逐条执行）
    │  通过函数指针调用（这些指针在 Init 里绑定好）
    ▼
收发函数  ── 这是你要写的"对接层"，把调用转成平台的 I2C API
（Tx/Rx × IT/DMA 共 4 个，示例里就是包一层 HAL 函数）
    │  硬件传输完成 / 出错 → 产生中断
    ▼
平台回调/中断（如 HAL_I2C_MasterTxCpltCallback）→ 转发给 KL_I2C_IT()
    ▼
执行你的完成回调（isSuccess = 1 成功 / 0 失败）→ 自动执行队列里下一个任务
```

## 移植一个平台：需要配置的东西（共 6 项）

所有答案都在示例文件里（`KL_I2C_conf.h`、`KL_I2C_Init.c`），下面逐条说明。

### 第 1 项：包含对应平台头文件（Conf）

打开 `KL_I2C_conf.h`，把 `#include` 换成你平台的 I2C 头文件。示例（STM32F4）：

```c
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
```

### 第 2 项：定义 `Hardware_I2C_HandleTypeDef` 封装结构体（Conf）

库不认识任何平台，它通过这个"中转结构体"间接访问你的 I2C 外设句柄。把你平台自己的 I2C 句柄塞进去就行：

```c
typedef struct Hardware_I2C_HandleTypeDef
{
    I2C_HandleTypeDef* I2C_Handle;   // ← 改成你平台的 I2C 句柄类型，比如 I2C_TypeDef*
    // 有多个不同类型外设（如 FMPI2C）就再加一个成员，示例里就是这么做的
} Hardware_I2C_HandleTypeDef;
```

> 如果平台只有一个 I2C 外设类型，这个结构体可以简单得只有一个成员。**这个结构体是核心句柄 `KL_I2C_HandleTypeDef` 里 `KL_I2C_hi2c`（i2c hardware 部分）指向的东西**，见第 5 项。

### 第 3 项：使能总线 + 队列长度 + DMA 开关（Conf）

```c
#define KL_I2C1_Enable              (1)   // 1 启用 / 0 禁用，用几条总线就开几条

#define KL_I2C1_Task_Queue_Len      (16)  // 任务队列长度，按业务量调整
#define KL_I2C1_USE_DMA_Tx          (0)   // 1 用 DMA 发送 / 0 用中断发送
#define KL_I2C1_USE_DMA_Rx          (0)   // 1 用 DMA 接收 / 0 用中断接收
```

- 用**中断方式**：前提是平台 I2C 中断已使能；实现对应 IT 收发函数即可（见第 4 项）。
- 用 **DMA 方式**：还需要把 DMA 通道配置好，并实现对应 DMA 收发函数。

### 第 4 项：实现 4 个收发函数（Init）

这是"接口跟库对接"的核心：库通过函数指针调用它们，你在函数体里调用平台的 I2C API。示例里是包一层 HAL：

```c
/* 中断发送 */
KL_Status KL_I2C_Transmit_IT(Hardware_I2C_HandleTypeDef* hw, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
    if (hw->I2C_Handle == NULL) return KL_ERROR;
    HAL_StatusTypeDef r = HAL_I2C_Master_Transmit_IT(hw->I2C_Handle, Addr, pData, Size);
    return (r == HAL_OK) ? KL_OK : KL_ERROR;
}

/* 中断接收 */
KL_Status KL_I2C_Receive_IT(Hardware_I2C_HandleTypeDef* hw, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
    if (hw->I2C_Handle == NULL) return KL_ERROR;
    HAL_StatusTypeDef r = HAL_I2C_Master_Receive_IT(hw->I2C_Handle, Addr, pData, Size);
    return (r == HAL_OK) ? KL_OK : KL_ERROR;
}

/* DMA 发送 / 接收同理，把上面的 IT 换成 DMA 的 API 即可 */
```

> 参数含义：`Addr` = 从机地址（原样透传给平台 API，按你平台的惯例传）、`pData` = 数据指针、`Size` = 字节数。
> **返回值必须转成 `KL_OK / KL_ERROR`**，库靠它判断任务是否成功入队。若平台没有 DMA，把对应函数返回 `KL_ERROR` 并把 DMA 开关关掉即可。

### 第 5 项：实例化句柄，把"i2c hardware 部分"绑好（Init）

在 `KL_I2C_Init.c` 里定义三个东西并**互相绑定**（示例文件写得明明白白）：

```c
/* ① 封装结构体实例：把平台句柄 &hi2c1（CubeMX 生成的）放进去 */
Hardware_I2C_HandleTypeDef KL_I2C1_Hardware = {
    .I2C_Handle = &hi2c1,      // ← 改为你平台实际使用的句柄
};

/* ② 任务队列数组 */
KL_I2C_COM_Task_Queue KL_I2C1_Task_Queue[KL_I2C1_Task_Queue_Len];

/* ③ 库句柄：KL_I2C_hi2c 指向 ①，收发函数指针挂上 ② ③ ④ 的队列和函数 */
KL_I2C_HandleTypeDef KL_I2C1_Handle = {
    .KL_I2C_hi2c        = &KL_I2C1_Hardware,  // ← i2c hardware 部分，指向封装结构体
    .KL_I2C_IT_Tx_Fun   = KL_I2C_Transmit_IT,
    .KL_I2C_IT_Rx_Fun   = KL_I2C_Receive_IT,
    .KL_I2C_DMA_Tx_Fun  = KL_I2C_Transmit_DMA,  // 不用 DMA 就填 NULL
    .KL_I2C_DMA_Rx_Fun  = KL_I2C_Receive_DMA,
    .Task_Queue         = KL_I2C1_Task_Queue,
    .KL_I2C_State_Register.bits = {
        .Task_Queue_Len = KL_I2C1_Task_Queue_Len,
        .USE_DMA_Tx_Flag = KL_I2C1_USE_DMA_Tx,  // 与 Conf 里保持一致
        .USE_DMA_Rx_Flag = KL_I2C1_USE_DMA_Rx,
        /* 其余位（R_Point / W_Point / Init_ed / State 等）保持示例里的初始值即可 */
    },
};
```

> 有多个总线就按同样模板复制成 `KL_I2C2_Handle`、`KL_I2C3_Handle`……（示例里已写好）。

### 第 6 项：初始化 + 中断事件转发（Init）

- **初始化**：实现 `KL_I2C_Hardware_Init()`，把 `Init_ed` 标志置 1（示例已写好，直接复用）。程序启动时调用一次：

  ```c
  KL_I2C_Init();   // 内部会调用 KL_I2C_Hardware_Init()
  ```

- **中断转发**：平台的"传输完成 / 出错"回调里，调用 `KL_I2C_IT(&对应Handle, isSuccess)`。示例（STM32 HAL）：

  ```c
  void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
  {
      if (hi2c->Instance == I2C1) KL_I2C_IT(&KL_I2C1_Handle, 1);  // 1 = 成功
  }
  void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
  {
      if (hi2c->Instance == I2C1) KL_I2C_IT(&KL_I2C1_Handle, 1);
  }
  void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
  {
      if (hi2c->Instance == I2C1) KL_I2C_IT(&KL_I2C1_Handle, 0);  // 0 = 失败
  }
  ```

> 非 STM32 平台：把上面三个回调换成你平台的中断服务函数 / 回调机制，逻辑一模一样——传输完成调 `KL_I2C_IT(handle, 1)`，出错调 `KL_I2C_IT(handle, 0)`。**漏掉这一步，任务无法自动执行。**

### 第7项：配置KL_IRQ_Enable(void)和KL_IRQ_Disable(void)

这两个函数主要用于多线程防重入。

```c
// ARM裸机示例
void KL_IRQ_Enable(void)
{
	__enable_irq();
}

void KL_IRQ_Disable(void)
{
	__disable_irq();
}
// FreeRTOS示例
void KL_IRQ_Enable(void)
{
	taskEXIT_CRITICAL();
}

void KL_IRQ_Disable(void)
{
	taskENTER_CRITICAL();
}
```

## STM32 + CubeMX 快速开始

1. **CubeMX 里**：开启 I2C1 外设 → 勾选 I2C1 全局中断（用 DMA 的话再配好 DMA 通道）；
2. 把 `Inc/`、`Src/` 加进工程；
3. 改 `KL_I2C_conf.h`：确认 `KL_I2Cx_Enable = 1`，按需调队列长度 / DMA 开关；
4. `KL_I2C_Init.c` 里确认 `&hi2cx 与 CubeMX 生成的句柄名一致 `；
5. `main()` 里调用 `KL_I2C_Init()`；
6. 开用。

## API 说明

| 函数 | 作用 |
|---|---|
| `KL_I2C_Init()` | 初始化所有使能的总线（内部调 `KL_I2C_Hardware_Init`） |
| `KL_I2C_RegisterTask(handle, MSG, Addr, buff, Len, Callback, arg)` | 注册一个读/写任务，立即返回，传输完成后自动调回调 |
| `KL_I2C_IT(handle, isSuccess)` | 中断转发入口，传输完成/出错时由你的中断回调调用（不要自己直接调） |

任务信息 `MSG` 的定义：

```c
KL_I2C_Task_MSG MSG;
MSG.bits.Type = KL_I2C_COM_Tra;  // KL_I2C_COM_Tra = 1 发送 / KL_I2C_COM_Rec = 0 接收
MSG.bits.Callback_Num = 1;       // 7 位任务识别码，回调里用它区分是哪个任务
```

## 使用示例

```c
#include "KL_I2C.h"

uint8_t tx_buf[2] = {0x00, 0x5A};
uint8_t rx_buf[4];

/* 任务的回调函数，用于同步任务进行的情况（没有也能正常运行） */
void I2C_Complete(KL_I2C_Task_MSG MSG, void* Handle, uint8_t isSuccess)
{
    if (MSG.bits.Type == KL_I2C_COM_Rec)
    {
        if (isSuccess) { /* 读取成功，rx_buf 里就是数据 */ }
        else           { /* 读取失败，可重试 */ }
    }
}

int main(void)
{
    /* ...平台初始化（含 CubeMX 生成的 I2C 初始化）... */
    KL_I2C_Init();

    /* 写：向从机地址 0xA0 发送 2 个字节 */
    KL_I2C_Task_MSG msg_w;
    msg_w.bits.Type = KL_I2C_COM_Tra;
    msg_w.bits.Callback_Num = 1;
    KL_I2C_RegisterTask(&KL_I2C1_Handle, msg_w, 0xA0, tx_buf, 2, I2C_Complete, NULL);

    /* 读：从 0xA0 读 4 个字节（不阻塞，立刻返回） */
    KL_I2C_Task_MSG msg_r;
    msg_r.bits.Type = KL_I2C_COM_Rec;
    msg_r.bits.Callback_Num = 2;
    KL_I2C_RegisterTask(&KL_I2C1_Handle, msg_r, 0xA0, rx_buf, 4, I2C_Complete, NULL);

    while (1) { /* 主循环不需要管 I2C，任务在中断里自动排队执行 */ }
}
```

## 常见问题

- **任务注册后回调永远不执行？** 99% 是第 6 项没做——平台传输完成中断没有转发给 `KL_I2C_IT()`，或者中断没使能。
- **回调里 `isSuccess` 一直为 0？** 检查硬件连接 / 从机地址（`Addr` 按平台惯例传，STM32 HAL 需要传 8 位地址，即 7 位地址左移 1 位）/ 收发函数返回值是否正确转成了 `KL_OK`。
- **`KL_I2C_RegisterTask` 返回 `KL_ERROR`？** 可能原因：`Len == 0`、回调为 `NULL`、还没调 `KL_I2C_Init()`、任务队列满了（加大 `Task_Queue_Len`）。
- **我的平台没有 DMA？** 把 `USE_DMA_Tx/Rx` 全设 0，DMA 函数填 `NULL`，只用中断收发即可。
- **`__weak` 是什么？** 核心文件里 `KL_I2C_Hardware_Init` 用了 `__weak`（弱定义），你在 Init 里强定义即可覆盖；GCC / ARMCC 都支持，其他编译器删掉 `__weak` 直接定义也一样。

## License

[MIT](LICENSE)

