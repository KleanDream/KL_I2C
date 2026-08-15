/* ============================================================================
					  KL_I2C + CubeMX Init配置规范示例文件
 ============================================================================*/
#ifndef _KL_I2C_Init_H_
#define _KL_I2C_Init_H_

#include "main.h"
#include "i2c.h"
#include "KL_I2C.h"
#include "KL_I2C_conf.h"

/* ===================I2C外设初始化部分支持================== */
/* 句柄命名规则兼容CubeMX风格 */
#if KL_FMPI2C_Enable == 1
#define KL_FMPI2C_hi2c		hfmpi2c1
#define KL_FMPI2C			FMPI2C1
/* FMPI2C传输完成中断函数声明 */
void FMPI2C_EV_IRQHandler(void);
void FMPI2C_ER_IRQHandler(void);
extern KL_I2C_HandleTypeDef KL_FMPI2C_Handle;
#endif

/* 句柄命名规则兼容CubeMX风格 */
#if KL_I2C1_Enable == 1
#define KL_I2C1_hi2c	hi2c1
#define KL_I2C1			I2C1
/* I2C1传输完成中断函数声明 */
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
extern KL_I2C_HandleTypeDef KL_I2C1_Handle;
#endif

/* 句柄命名规则兼容CubeMX风格 */
#if KL_I2C2_Enable == 1
#define KL_I2C2_hi2c	hi2c2
#define KL_I2C2			I2C2
/* I2C2传输完成中断函数声明 */
void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
extern KL_I2C_HandleTypeDef KL_I2C2_Handle;
#endif

/* 句柄命名规则兼容CubeMX风格 */
#if KL_I2C3_Enable == 1
#define KL_I2C3_hi2c	hi2c3
#define KL_I2C3			I2C3
/* I2C3传输完成中断函数声明 */
void I2C3_EV_IRQHandler(void);
void I2C3_ER_IRQHandler(void);
extern KL_I2C_HandleTypeDef KL_I2C3_Handle;
#endif
/* ========================================================== */
#if KL_FMPI2C_Enable == 1
KL_Status KL_FMPI2C_Transmit_DMA(Hardware_I2C_HandleTypeDef* KL_FMPI2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size);
KL_Status KL_FMPI2C_Transmit_IT(Hardware_I2C_HandleTypeDef* KL_FMPI2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size);
KL_Status KL_FMPI2C_Receive_DMA(Hardware_I2C_HandleTypeDef* KL_FMPI2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size);
KL_Status KL_FMPI2C_Receive_IT(Hardware_I2C_HandleTypeDef* KL_FMPI2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size);
#endif
KL_Status KL_I2C_Transmit_DMA(Hardware_I2C_HandleTypeDef* KL_I2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size);
KL_Status KL_I2C_Transmit_IT(Hardware_I2C_HandleTypeDef* KL_I2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size);
KL_Status KL_I2C_Receive_DMA(Hardware_I2C_HandleTypeDef* KL_I2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size);
KL_Status KL_I2C_Receive_IT(Hardware_I2C_HandleTypeDef* KL_I2C_Hardware, uint16_t Addr, uint8_t *pData, uint16_t Size);

#endif
