/* ============================================================================
					  KL_I2C + CubeMX Init配置规范示例文件
 ============================================================================*/

#include "KL_I2C.h"
#include "KL_I2C_Init.h"

// 定义I2C总线句柄和任务队列
#if KL_FMPI2C_Enable == 1
	Hardware_I2C_HandleTypeDef KL_FMPI2C_Hardware = {
		.I2C_Handle = NULL,
		.FMPI2C_Handle = &KL_FMPI2C_hi2c,
	};
	KL_I2C_COM_Task_Queue KL_FMPI2C_Task_Queue[KL_FMPI2C_Task_Queue_Len];
	KL_I2C_HandleTypeDef KL_FMPI2C_Handle = {
		.KL_I2C_hi2c->FMPI2C_Handle = &KL_FMPI2C_hi2c,
		.KL_I2C_IT_Tx_Fun = KL_FMPI2C_Transmit_IT,
		.KL_I2C_IT_Rx_Fun = KL_FMPI2C_Receive_IT,
		#if  KL_FMPI2C_USE_DMA_Tx == 1
		.KL_I2C_DMA_Tx_Fun = KL_FMPI2C_Transmit_DMA,
		#else
		.KL_I2C_DMA_Tx_Fun = NULL,
		#endif
		#if  KL_FMPI2C_USE_DMA_Rx == 1
		.KL_I2C_DMA_Rx_Fun = KL_FMPI2C_Receive_DMA,
		#else
		.KL_I2C_DMA_Rx_Fun = NULL,
		#endif
		.Task_Queue = KL_FMPI2C_Task_Queue,
		.KL_I2C_State_Register.bits = {
			.Task_Queue_Len = KL_FMPI2C_Task_Queue_Len,
			.R_Point = 0,
			.W_Point = 0,
			.First_Write = 0,
			.First_Read = 0,
			.Init_ed = 0,
			.State = 0,
			.USE_DMA_Tx_Flag = KL_FMPI2C_USE_DMA_Tx,
			.USE_DMA_Rx_Flag = KL_FMPI2C_USE_DMA_Rx,
		},
	};
#endif

#if KL_I2C1_Enable == 1
	Hardware_I2C_HandleTypeDef KL_I2C1_Hardware = {
		.I2C_Handle = &KL_I2C1_hi2c,
		#if KL_FMPI2C_Enable == 1
		.FMPI2C_Handle = NULL,
		#endif
	};
	KL_I2C_COM_Task_Queue KL_I2C1_Task_Queue[KL_I2C1_Task_Queue_Len];
	KL_I2C_HandleTypeDef KL_I2C1_Handle = {
		.KL_I2C_hi2c = &KL_I2C1_Hardware,
		.KL_I2C_IT_Tx_Fun = KL_I2C_Transmit_IT,
		.KL_I2C_IT_Rx_Fun = KL_I2C_Receive_IT,
		#if  KL_I2C1_USE_DMA_Tx == 1
		.KL_I2C_DMA_Tx_Fun = KL_I2C_Transmit_DMA,
		#else
		.KL_I2C_DMA_Tx_Fun = NULL,
		#endif
		#if  KL_I2C1_USE_DMA_Rx == 1
		.KL_I2C_DMA_Rx_Fun = KL_I2C_Receive_DMA,
		#else
		.KL_I2C_DMA_Rx_Fun = NULL,
		#endif
		.Task_Queue = KL_I2C1_Task_Queue,
		.KL_I2C_State_Register.bits = {
			.Task_Queue_Len = KL_I2C1_Task_Queue_Len,
			.R_Point = 0,
			.W_Point = 0,
			.First_Write = 0,
			.First_Read = 0,
			.Init_ed = 0,
			.State = 0,
			.USE_DMA_Tx_Flag = KL_I2C1_USE_DMA_Tx,
			.USE_DMA_Rx_Flag = KL_I2C1_USE_DMA_Rx,
		},
	};
#endif

#if KL_I2C2_Enable == 1
	Hardware_I2C_HandleTypeDef KL_I2C2_Hardware = {
		.I2C_Handle = &KL_I2C2_hi2c,
		#if KL_FMPI2C_Enable == 1
		.FMPI2C_Handle = NULL,
		#endif
	};
	KL_I2C_COM_Task_Queue KL_I2C2_Task_Queue[KL_I2C2_Task_Queue_Len];
	KL_I2C_HandleTypeDef KL_I2C2_Handle = {
		.KL_I2C_hi2c = &KL_I2C2_Hardware,
		.KL_I2C_IT_Tx_Fun = KL_I2C_Transmit_IT,
		.KL_I2C_IT_Rx_Fun = KL_I2C_Receive_IT,
		#if  KL_I2C2_USE_DMA_Tx == 1
		.KL_I2C_DMA_Tx_Fun = KL_I2C_Transmit_DMA,
		#else
		.KL_I2C_DMA_Tx_Fun = NULL,
		#endif
		#if  KL_I2C2_USE_DMA_Rx == 1
		.KL_I2C_DMA_Rx_Fun = KL_I2C_Receive_DMA,
		#else
		.KL_I2C_DMA_Rx_Fun = NULL,
		#endif
		.Task_Queue = KL_I2C2_Task_Queue,
		.KL_I2C_State_Register.bits = {
			.Task_Queue_Len = KL_I2C2_Task_Queue_Len,
			.R_Point = 0,
			.W_Point = 0,
			.First_Write = 0,
			.First_Read = 0,
			.Init_ed = 0,
			.State = 0,
			.USE_DMA_Tx_Flag = KL_I2C2_USE_DMA_Tx,
			.USE_DMA_Rx_Flag = KL_I2C2_USE_DMA_Rx,
		},
	};
#endif

#if KL_I2C3_Enable == 1
	Hardware_I2C_HandleTypeDef KL_I2C3_Hardware = {
		.I2C_Handle = &KL_I2C3_hi2c,
		#if KL_FMPI2C_Enable == 1
		.FMPI2C_Handle = NULL,
		#endif
	};
	KL_I2C_COM_Task_Queue KL_I2C3_Task_Queue[KL_I2C3_Task_Queue_Len];
	KL_I2C_HandleTypeDef KL_I2C3_Handle = {
		.KL_I2C_hi2c = &KL_I2C3_Hardware,
		.KL_I2C_IT_Tx_Fun = KL_I2C_Transmit_IT,
		.KL_I2C_IT_Rx_Fun = KL_I2C_Receive_IT,
		#if  KL_I2C3_USE_DMA_Tx == 1
		.KL_I2C_DMA_Tx_Fun = KL_I2C_Transmit_DMA,
		#else
		.KL_I2C_DMA_Tx_Fun = NULL,
		#endif
		#if  KL_I2C3_USE_DMA_Rx == 1
		.KL_I2C_DMA_Rx_Fun = KL_I2C_Receive_DMA,
		#else
		.KL_I2C_DMA_Rx_Fun = NULL,
		#endif
		.Task_Queue = KL_I2C3_Task_Queue,
		.KL_I2C_State_Register.bits = {
			.Task_Queue_Len = KL_I2C3_Task_Queue_Len,
			.R_Point = 0,
			.W_Point = 0,
			.First_Write = 0,
			.First_Read = 0,
			.Init_ed = 0,
			.State = 0,
			.USE_DMA_Tx_Flag = KL_I2C3_USE_DMA_Tx,
			.USE_DMA_Rx_Flag = KL_I2C3_USE_DMA_Rx,
		},
	};
#endif

/** 
  * 需要实现至少一种I2C的收发函数
  * 以下为基于STM32 HAL库时需要的封装
  */
#if KL_FMPI2C_Enable == 1
/* FMPI2C通过DMA方法发送数据  */
KL_Status KL_FMPI2C_Transmit_DMA(Hardware_I2C_HandleTypeDef* KL_FMPI2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
	if(KL_FMPI2C_Hardware->FMPI2C_Handle == NULL) return KL_ERROR;
	HAL_StatusTypeDef COM_result = HAL_FMPI2C_Master_Transmit_DMA(KL_FMPI2C_Hardware->FMPI2C_Handle, Addr, pData, Size);

	if(COM_result == HAL_OK)
		return KL_OK;
	return KL_ERROR;
}

/* FMPI2C通过中断方法发送数据 */
KL_Status KL_FMPI2C_Transmit_IT(Hardware_I2C_HandleTypeDef* KL_FMPI2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
	if(KL_FMPI2C_Hardware->FMPI2C_Handle == NULL) return KL_ERROR;
	HAL_StatusTypeDef COM_result = HAL_FMPI2C_Master_Transmit_IT(KL_FMPI2C_hi2c, Addr, pData, Size);

	if(COM_result == HAL_OK)
		return KL_OK;
	return KL_ERROR;
}

/* FMPI2C通过DMA方法接收数据  */
KL_Status KL_FMPI2C_Receive_DMA(Hardware_I2C_HandleTypeDef* KL_FMPI2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
	if(KL_FMPI2C_Hardware->FMPI2C_Handle == NULL) return KL_ERROR;
	HAL_StatusTypeDef COM_result = HAL_FMPI2C_Master_Receive_DMA(KL_FMPI2C_hi2c, Addr, pData, Size);

	if(COM_result == HAL_OK)
		return KL_OK;
	return KL_ERROR;
}

/* FMPI2C通过中断方法接收数据 */
KL_Status KL_FMPI2C_Receive_IT(Hardware_FMPI2C_HandleTypeDef* KL_FMPI2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
	if(KL_FMPI2C_Hardware->FMPI2C_Handle == NULL) return KL_ERROR;
	HAL_StatusTypeDef COM_result = HAL_FMPI2C_Master_Receive_IT(KL_FMPI2C_hi2c, Addr, pData, Size);

	if(COM_result == HAL_OK)
		return KL_OK;
	return KL_ERROR;
}
#endif

/* I2C通过DMA方法发送数据  */
KL_Status KL_I2C_Transmit_DMA(Hardware_I2C_HandleTypeDef* KL_I2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
	if(KL_I2C_Hardware->I2C_Handle == NULL) return KL_ERROR;
	HAL_StatusTypeDef COM_result = HAL_I2C_Master_Transmit_DMA(KL_I2C_Hardware->I2C_Handle, Addr, pData, Size);

	if(COM_result == HAL_OK)
		return KL_OK;
	return KL_ERROR;
}

/* I2C通过中断方法发送数据 */
KL_Status KL_I2C_Transmit_IT(Hardware_I2C_HandleTypeDef* KL_I2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
	if(KL_I2C_Hardware->I2C_Handle == NULL) return KL_ERROR;
	HAL_StatusTypeDef COM_result = HAL_I2C_Master_Transmit_IT(KL_I2C_Hardware->I2C_Handle, Addr, pData, Size);

	if(COM_result == HAL_OK)
		return KL_OK;
	return KL_ERROR;
}

/* I2C通过DMA方法接收数据  */
KL_Status KL_I2C_Receive_DMA(Hardware_I2C_HandleTypeDef* KL_I2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
	if(KL_I2C_Hardware->I2C_Handle == NULL) return KL_ERROR;
	HAL_StatusTypeDef COM_result = HAL_I2C_Master_Receive_DMA(KL_I2C_Hardware->I2C_Handle, Addr, pData, Size);

	if(COM_result == HAL_OK)
		return KL_OK;
	return KL_ERROR;
}

/* I2C通过中断方法接收数据 */
KL_Status KL_I2C_Receive_IT(Hardware_I2C_HandleTypeDef* KL_I2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size)
{
	if(KL_I2C_Hardware->I2C_Handle == NULL) return KL_ERROR;
	HAL_StatusTypeDef COM_result = HAL_I2C_Master_Receive_IT(KL_I2C_Hardware->I2C_Handle, Addr, pData, Size);

	if(COM_result == HAL_OK)
		return KL_OK;
	return KL_ERROR;
}

/* 你可以自己定制的硬件init方法，但一定记住完成后设置Init_ed状态 */
KL_Status KL_I2C_Hardware_Init(void)
{
	#if KL_FMPI2C_Enable == 1
	KL_FMPI2C_Handle.KL_I2C_State_Register.bits.Init_ed = 1;
	#endif
	#if KL_I2C1_Enable == 1
	KL_I2C1_Handle.KL_I2C_State_Register.bits.Init_ed = 1;
	#endif
	#if KL_I2C2_Enable == 1
	KL_I2C2_Handle.KL_I2C_State_Register.bits.Init_ed = 1;
	#endif
	#if KL_I2C3_Enable == 1
	KL_I2C3_Handle.KL_I2C_State_Register.bits.Init_ed = 1;
	#endif
	return KL_OK;
}

//中断回调函数配置
#if KL_FMPI2C_Enable == 1
void HAL_FMPI2C_MasterTxCpltCallback(FMPI2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == KL_FMPI2C)
    {
		KL_I2C_IT(&KL_FMPI2C_Handle, 1);
    }
}

void HAL_FMPI2C_MasterRxCpltCallback(FMPI2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == KL_FMPI2C)
    {
		KL_I2C_IT(&KL_FMPI2C_Handle, 1);
    }
}

void HAL_FMPI2C_ErrorCallback(FMPI2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == KL_FMPI2C)
    {
		KL_I2C_IT(&KL_FMPI2C_Handle, 0);
    }
}
#endif

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	#if KL_I2C1_Enable == 1
    if (hi2c->Instance == KL_I2C1)
    {
		KL_I2C_IT(&KL_I2C1_Handle, 1);
    }
	#endif
	#if KL_I2C2_Enable == 1
	if (hi2c->Instance == KL_I2C2)
    {
		KL_I2C_IT(&KL_I2C2_Handle, 1);
    }
	#endif
	#if KL_I2C3_Enable == 1
	if (hi2c->Instance == KL_I2C3)
    {
		KL_I2C_IT(&KL_I2C3_Handle, 1);
    }
	#endif
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	#if KL_I2C1_Enable == 1
    if (hi2c->Instance == KL_I2C1)
    {
		KL_I2C_IT(&KL_I2C1_Handle, 1);
    }
	#endif
	#if KL_I2C2_Enable == 1
	if (hi2c->Instance == KL_I2C2)
    {
		KL_I2C_IT(&KL_I2C2_Handle, 1);
    }
	#endif
	#if KL_I2C3_Enable == 1
	if (hi2c->Instance == KL_I2C3)
    {
		KL_I2C_IT(&KL_I2C3_Handle, 1);
    }
	#endif
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	#if KL_I2C1_Enable == 1
    if (hi2c->Instance == KL_I2C1)
    {
		KL_I2C_IT(&KL_I2C1_Handle, 0);
    }
	#endif
	#if KL_I2C2_Enable == 1
	if (hi2c->Instance == KL_I2C2)
    {
		KL_I2C_IT(&KL_I2C2_Handle, 0);
    }
	#endif
	#if KL_I2C3_Enable == 1
	if (hi2c->Instance == KL_I2C3)
    {
		KL_I2C_IT(&KL_I2C3_Handle, 0);
    }
	#endif
}

