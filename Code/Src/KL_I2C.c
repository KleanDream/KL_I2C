/* 通用型I2C总线控制器 LKL制作 */
/* 可实现完全无阻塞式通信 */
/* 本驱动适用于F1系列和F4系列，其他系列没用过所以没法做适配 */

#include "KL_I2C.h"
#include "KL_I2C_Init.h"

void KL_IRQ_Enable(void);
void KL_IRQ_Disable(void);

static KL_Status KL_I2C_ExecuteTask(KL_I2C_HandleTypeDef* KL_I2C_Handle);

/* 注册传输任务 */
KL_Status KL_I2C_RegisterTask(KL_I2C_HandleTypeDef* KL_I2C_Handle, KL_I2C_Task_MSG MSG, uint8_t Addr, uint8_t* buff, uint16_t Len, KL_I2C_Complete_Callback Callback_Fun, void* Handle)
{
	// Len不应为0，返回错误码
	if(Len == 0) return KL_ERROR;
	// 防止回调函数不正常
	if(Callback_Fun == NULL) return KL_ERROR;
	// 如果未完成初始化，返回错误码
	if(!KL_I2C_Handle->KL_I2C_State_Register.bits.Init_ed) return KL_ERROR;
	// 判断是否是该总线的第一个任务 0为是 1为否
	// 如果是第一个任务不用指针自增直接写入
	KL_IRQ_Disable();
	if(KL_I2C_Handle->KL_I2C_State_Register.bits.First_Write) 
	{
		if(KL_I2C_Handle->KL_I2C_State_Register.bits.W_Point >= KL_I2C_Handle->KL_I2C_State_Register.bits.Task_Queue_Len - 1) KL_I2C_Handle->KL_I2C_State_Register.bits.W_Point = 0;
		else KL_I2C_Handle->KL_I2C_State_Register.bits.W_Point++;
		if(KL_I2C_Handle->KL_I2C_State_Register.bits.W_Point == KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point)
		{	// 任务队列炸了，恢复指针原来的值，返回错误码
			if(KL_I2C_Handle->KL_I2C_State_Register.bits.W_Point == 0) KL_I2C_Handle->KL_I2C_State_Register.bits.W_Point = KL_I2C_Handle->KL_I2C_State_Register.bits.Task_Queue_Len - 1;
			else KL_I2C_Handle->KL_I2C_State_Register.bits.W_Point--;
			KL_IRQ_Enable();
			return KL_ERROR;
		}
	}
	else KL_I2C_Handle->KL_I2C_State_Register.bits.First_Write = 1;
	uint16_t W_Point = KL_I2C_Handle->KL_I2C_State_Register.bits.W_Point;
	KL_I2C_Handle->Task_Queue[W_Point].Task_Data_Sync_Flag = 0;
	KL_IRQ_Enable();
	KL_I2C_Handle->Task_Queue[W_Point].Addr = Addr;
	KL_I2C_Handle->Task_Queue[W_Point].buff = buff;
	KL_I2C_Handle->Task_Queue[W_Point].Len = Len;
	KL_I2C_Handle->Task_Queue[W_Point].MSG = MSG;
	KL_I2C_Handle->Task_Queue[W_Point].Callback_Fun = Callback_Fun;
	KL_I2C_Handle->Task_Queue[W_Point].Handle = Handle;
	KL_I2C_Handle->Task_Queue[W_Point].Task_Data_Sync_Flag = 1;
	KL_I2C_ExecuteTask(KL_I2C_Handle);
	return KL_OK;
}

/* 执行传输任务 */
static KL_Status KL_I2C_ExecuteTask(KL_I2C_HandleTypeDef* KL_I2C_Handle)
{
	// 如果未完成初始化，返回错误码
	if(!KL_I2C_Handle->KL_I2C_State_Register.bits.Init_ed) return KL_ERROR;
	// 判断该总线是否注册过任务 0为否 1为是
	// 如果没有注册过任务则无法执行，返回错误码
	if(!KL_I2C_Handle->KL_I2C_State_Register.bits.First_Write) return KL_ERROR;
	// 如果处于忙状态则无法执行任务，返回总线繁忙标志码
	KL_IRQ_Disable();
	if(KL_I2C_Handle->KL_I2C_State_Register.bits.State)
	{
		KL_IRQ_Enable();
		return KL_BUSY;
	}
	else KL_I2C_Handle->KL_I2C_State_Register.bits.State = 1;
	KL_IRQ_Enable();
	// 判断是否是该总线的第一个任务 0为是 1为否
	// 如果是第一个任务不用指针自增直接执行
	if(KL_I2C_Handle->KL_I2C_State_Register.bits.First_Read) 
	{	// 如果R_Point追上W_Point则意味着没有任务可以做了，返回错误码
		if(KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point == KL_I2C_Handle->KL_I2C_State_Register.bits.W_Point)
		{
			KL_I2C_Handle->KL_I2C_State_Register.bits.State = 0;
			return KL_ERROR;
		}
		if(KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point >= KL_I2C_Handle->KL_I2C_State_Register.bits.Task_Queue_Len - 1) KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point = 0;
		else KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point++;
		// 判断任务数据完整性
		if(!KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Task_Data_Sync_Flag)
		{	// 任务数据不完整，恢复指针原来的值，返回错误码
			if(KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point == 0) KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point = KL_I2C_Handle->KL_I2C_State_Register.bits.Task_Queue_Len - 1;
			else KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point--;
			KL_I2C_Handle->KL_I2C_State_Register.bits.State = 0;
			return KL_ERROR;
		}
	}
	else
	{	// 判断任务数据完整性
		if(KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Task_Data_Sync_Flag) KL_I2C_Handle->KL_I2C_State_Register.bits.First_Read = 1;
		else
		{	// 任务数据不完整，返回错误码
			KL_I2C_Handle->KL_I2C_State_Register.bits.State = 0;
			return KL_ERROR;
		}
	}
	
	//判断任务类型(发送/读取)
	if(KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].MSG.bits.Type == KL_I2C_COM_Rec)
	{	//	读取任务
		if(KL_I2C_Handle->KL_I2C_State_Register.bits.USE_DMA_Rx_Flag)
		{
			if(KL_I2C_Handle->KL_I2C_DMA_Rx_Fun(KL_I2C_Handle->KL_I2C_hi2c,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Addr,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].buff,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Len) != KL_OK)
			{	// 传输失败则返回错误码5
				//退出忙状态
				KL_I2C_Handle->KL_I2C_State_Register.bits.State = 0;
				return KL_ERROR;
			}
		}
		else
		{
			if(KL_I2C_Handle->KL_I2C_IT_Rx_Fun(	KL_I2C_Handle->KL_I2C_hi2c,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Addr,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].buff,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Len) != KL_OK)
			{	// 传输失败则返回错误码5
				//退出忙状态
				KL_I2C_Handle->KL_I2C_State_Register.bits.State = 0;
				return KL_ERROR;
			}
		}
	}
	else if(KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].MSG.bits.Type == KL_I2C_COM_Tra)
	{	//	发送任务
		if(KL_I2C_Handle->KL_I2C_State_Register.bits.USE_DMA_Tx_Flag)
		{
			if(KL_I2C_Handle->KL_I2C_DMA_Tx_Fun(KL_I2C_Handle->KL_I2C_hi2c,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Addr,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].buff,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Len) != KL_OK)
			{	// 传输失败则返回错误码5
				//退出忙状态
				KL_I2C_Handle->KL_I2C_State_Register.bits.State = 0;
				return KL_ERROR;
			}
		}
		else
		{
			if(KL_I2C_Handle->KL_I2C_IT_Tx_Fun(	KL_I2C_Handle->KL_I2C_hi2c,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Addr,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].buff,
												KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Len) != KL_OK)
			{	// 传输失败则返回错误码5
				//退出忙状态
				KL_I2C_Handle->KL_I2C_State_Register.bits.State = 0;
				return KL_ERROR;
			}
		}
	}
	return KL_OK;
}

/* 数据传输完成回调函数 */
//在HAL_I2C_MasterTxCpltCallback中调用此函数
void KL_I2C_IT(KL_I2C_HandleTypeDef* KL_I2C_Handle, uint8_t isSuccess)
{
	KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point]
		.Callback_Fun(	KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].MSG,
						KL_I2C_Handle->Task_Queue[KL_I2C_Handle->KL_I2C_State_Register.bits.R_Point].Handle,
						isSuccess);
	//退出忙状态
	KL_I2C_Handle->KL_I2C_State_Register.bits.State = 0;
	KL_I2C_ExecuteTask(KL_I2C_Handle);
}

__weak KL_Status KL_I2C_Hardware_Init(void)
{
	return KL_OK;
}

KL_Status KL_I2C_Init(void)
{
	if(KL_I2C_Hardware_Init() != KL_OK) return KL_ERROR;
	return KL_OK;
}

