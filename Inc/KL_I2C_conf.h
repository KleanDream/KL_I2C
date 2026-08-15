/* ============================================================================
					   KL_I2C + CubeMX conf配置规范示例文件
 ============================================================================*/
#ifndef _KL_I2C_CONF_H_
#define _KL_I2C_CONF_H_

/* 包含对应平台的头文件 */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* 当芯片中有多个I2C外设需要管理时的示例 */
/* 硬件I2C线路使用情况 */
/* 适配部分型号FMPI2C  */
#define KL_FMPI2C_Enable				(0)
#define KL_I2C1_Enable					(1)
#define KL_I2C2_Enable					(0)
#define KL_I2C3_Enable					(0)

/* 硬件I2C外设句柄结构体绑定 */
// 下方示例适配STM32F4平台
typedef struct Hardware_I2C_HandleTypeDef
{
	I2C_HandleTypeDef* I2C_Handle;
	#if KL_FMPI2C_Enable == 1
	FMPI2C_HandleTypeDef* FMPI2C_Handle;
	#endif
} Hardware_I2C_HandleTypeDef;

/* 增强性I2C总线配置 */
#if KL_FMPI2C_Enable == 1
#define KL_FMPI2C_Task_Queue_Len			(16)	// 任务队列长度
#define KL_FMPI2C_USE_DMA_Tx				(1)		// DMA外设选择(Tx)
#define KL_FMPI2C_USE_DMA_Rx				(1)		// DMA外设选择(Rx)
#endif

/* I2C1总线配置 */
#if KL_I2C1_Enable == 1
#define KL_I2C1_Task_Queue_Len				(16)	// 任务队列长度
#define KL_I2C1_USE_DMA_Tx					(0)		// DMA外设选择(Tx)
#define KL_I2C1_USE_DMA_Rx					(0)		// DMA外设选择(Rx)
#endif

/* I2C2总线配置 */
#if KL_I2C2_Enable == 1
#define KL_I2C2_Task_Queue_Len				(16)	// 任务队列长度
#define KL_I2C2_USE_DMA_Tx					(1)		// DMA外设选择(Tx)
#define KL_I2C2_USE_DMA_Rx					(1)		// DMA外设选择(Rx)
#endif

/* I2C3总线配置 */
#if KL_I2C3_Enable == 1
#define KL_I2C3_Task_Queue_Len				(16)	// 任务队列长度
#define KL_I2C3_USE_DMA_Tx					(0)		// DMA外设选择(Tx)
#define KL_I2C3_USE_DMA_Rx					(0)		// DMA外设选择(Rx)
#endif

#endif
