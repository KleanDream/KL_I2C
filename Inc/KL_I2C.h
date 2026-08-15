#ifndef _KL_I2C_H_
#define _KL_I2C_H_

#include "stdint.h"
#include "KL_I2C_conf.h"

/* ==================全异步任务管理部分支持================== */
//读取与发送指令
#define KL_I2C_COM_Rec		0
#define KL_I2C_COM_Tra		1

/* =============任务信息结构体============= */
typedef union{
	struct {
		uint8_t	Type				: 1;	// 任务类型(0为接收 1为发送)
		uint8_t	Callback_Num		: 7;	// 7位任务识别码
	} bits;
	uint8_t byte;         // 整体访问，主要是清空时使用
}KL_I2C_Task_MSG;
/* ======================================== */

/* 任务完成回调函数指针 */
typedef void (*KL_I2C_Complete_Callback)(KL_I2C_Task_MSG MSG, void* Handle, uint8_t isSuccess);

/* =============任务信息结构体============= */
typedef struct KL_I2C_COM_Task_Queue
{
	KL_I2C_Task_MSG				MSG;
	uint8_t						Task_Data_Sync_Flag;
	uint16_t					Addr;
	uint16_t					Len;
	uint8_t*					buff;
	KL_I2C_Complete_Callback	Callback_Fun;
	void* 						Handle;
}KL_I2C_COM_Task_Queue;
/* ======================================== */

/* ==============I2C总线状态机============= */
typedef union{
	struct {
		uint32_t R_Point			: 8;	// I2C总线任务进行指针
		uint32_t W_Point			: 8;	// I2C总线任务加入指针
		uint32_t First_Write		: 1;	// 初次写入标志位
		uint32_t First_Read			: 1;	// 初次读取标志位
		uint32_t Init_ed			: 1;	// 初始化完成标志位
		uint32_t State				: 1;	// 总线1状态(0空闲/1忙)
		/* ======静态参数====== */
		uint32_t Task_Queue_Len		: 8;	// 任务队列长度
		uint32_t USE_DMA_Tx_Flag	: 1;	// DMA_Tx使能标志位
		uint32_t USE_DMA_Rx_Flag	: 1;	// DMA_Rx使能标志位
		uint32_t reserved			: 2;	// 保留位
	} bits;
	uint32_t Register;         // 整体访问，仅清空时使用
} volatile KL_I2C_State;
/* ======================================== */

typedef enum{
	KL_OK		= 0,
	KL_ERROR	= 1,
	KL_BUSY		= 2,
} volatile KL_Status;

typedef KL_Status (*KL_I2C_Tx_DMA)(Hardware_I2C_HandleTypeDef* KL_I2C_hi2c, uint16_t Addr, uint8_t *pData, uint16_t Size);
typedef KL_Status (*KL_I2C_Tx_IT)(Hardware_I2C_HandleTypeDef* KL_I2C_hi2c, uint16_t Addr, uint8_t *pData, uint16_t Size);
typedef KL_Status (*KL_I2C_Rx_DMA)(Hardware_I2C_HandleTypeDef* KL_I2C_hi2c, uint16_t Addr, uint8_t *pData, uint16_t Size);
typedef KL_Status (*KL_I2C_Rx_IT)(Hardware_I2C_HandleTypeDef* KL_I2C_hi2c, uint16_t Addr, uint8_t *pData, uint16_t Size);

/* ===============KL库I2C句柄============== */
typedef struct KL_I2C_HandleTypeDef
{
	Hardware_I2C_HandleTypeDef*	KL_I2C_hi2c;
	KL_I2C_Tx_DMA				KL_I2C_DMA_Tx_Fun;
	KL_I2C_Tx_IT				KL_I2C_IT_Tx_Fun;
	KL_I2C_Rx_DMA				KL_I2C_DMA_Rx_Fun;
	KL_I2C_Rx_IT				KL_I2C_IT_Rx_Fun;
	KL_I2C_COM_Task_Queue*		Task_Queue;
	KL_I2C_State				KL_I2C_State_Register;
}KL_I2C_HandleTypeDef;
/* ======================================== */

/* 注册传输任务 */
KL_Status KL_I2C_RegisterTask(KL_I2C_HandleTypeDef* KL_I2C_Handle, KL_I2C_Task_MSG MSG, uint8_t Addr, uint8_t* buff, uint16_t Len, KL_I2C_Complete_Callback Callback_Fun, void* Handle);

/* 任务回调函数 */
void KL_I2C_IT(KL_I2C_HandleTypeDef* KL_I2C_Handle, uint8_t isSuccess);
/* ========================================================== */
KL_Status KL_I2C_Hardware_Init(void);
KL_Status KL_I2C_Init(void);

#endif
