/*
 * ICM20608g.h
 *
 *  Created on: 15 Ağu 2023
 *      Author: KRAKER
 */

#ifndef ICM20608g_ICM20608g_H_
#define ICM20608g_ICM20608g_H_

#include "main_app.h"


I2C_HandleTypeDef 	hICM20608g_I2C;

/**
 * @brief Definition for connected to I2C1 (APB2 PCLK = 25MHz)
 */
#define ICM20608g_I2C_CHANNEL		I2C1

#define ICM20608g_I2C_SCL_PIN		GPIO_PIN_6
#define ICM20608g_I2C_SCL_PORT		GPIOB

#define ICM20608g_I2C_SDA_PIN		GPIO_PIN_9
#define ICM20608g_I2C_SDA_PORT		GPIOB

#define	ICM20608g_I2C_ADDRESS		0xD0


/**
 * @brief ICM-20602 Register Map
 */
#define	XG_OFFS_TC_H			0x04
#define	XG_OFFS_TC_L			0x05
#define	YG_OFFS_TC_H			0x07
#define	YG_OFFS_TC_L			0x08
#define	ZG_OFFS_TC_H			0x0A
#define	ZG_OFFS_TC_L			0x0B
#define	SELF_TEST_X_ACCEL		0x0D
#define	SELF_TEST_Y_ACCEL		0x0E
#define	SELF_TEST_Z_ACCEL		0x0F
#define	XG_OFFS_USRH			0x13
#define	XG_OFFS_USRL			0x14
#define	YG_OFFS_USRH			0x15
#define	YG_OFFS_USRL			0x16
#define	ZG_OFFS_USRH			0x17
#define	ZG_OFFS_USRL			0x18
#define	SMPLRT_DIV				0x19
#define	CONFIG					0x1A
#define	GYRO_CONFIG				0x1B
#define	ACCEL_CONFIG			0x1C
#define	ACCEL_CONFIG2			0x1D
#define	LP_MODE_CFG				0x1E
#define	ACCEL_WOM_X_THR			0x20
#define	ACCEL_WOM_Y_THR			0x21
#define	ACCEL_WOM_Z_THR			0x22
#define	FIFO_EN					0x23
#define	FSYNC_INT				0x36
#define	INT_PIN_CFG				0x37
#define	INT_ENABLE				0x38
#define	FIFO_WM_INT_STATUS		0x39

#define	INT_STATUS				0x3A
#define	ACCEL_XOUT_H			0x3B
#define	ACCEL_XOUT_L			0x3C
#define	ACCEL_YOUT_H			0x3D
#define	ACCEL_YOUT_L			0x3E
#define	ACCEL_ZOUT_H			0x3F
#define	ACCEL_ZOUT_L			0x40
#define	TEMP_OUT_H				0x41
#define	TEMP_OUT_L				0x42
#define	GYRO_XOUT_H				0x43
#define	GYRO_XOUT_L				0x44
#define	GYRO_YOUT_H				0x45
#define	GYRO_YOUT_L				0x46
#define	GYRO_ZOUT_H				0x47
#define	GYRO_ZOUT_L				0x48
#define	SELF_TEST_X_GYRO		0x50
#define	SELF_TEST_Y_GYRO		0x51
#define	SELF_TEST_Z_GYRO		0x52
#define	FIFO_WM_TH1				0x60
#define	FIFO_WM_TH2				0x61
#define	SIGNAL_PATH_RESET		0x68
#define	ACCEL_INTEL_CTRL		0x69
#define	USER_CTRL				0x6A
#define	PWR_MGMT_1				0x6B //The default value of the register is 0x40.
#define	PWR_MGMT_2				0x6C
#define	I2C_IF					0x70
#define	FIFO_COUNTH				0x72
#define	FIFO_COUNTL				0x73
#define	FIFO_R_W				0x74
#define	WHO_AM_I				0x75 //The default value of the register is 0xAF.
#define	XA_OFFSET_H				0x77
#define	XA_OFFSET_L				0x78
#define	YA_OFFSET_H				0x7A
#define	YA_OFFSET_L				0x7B
#define	ZA_OFFSET_H				0x7D
#define	ZA_OFFSET_L				0x7E




/**
 * @brief ICM20608g structure definition.
 */

typedef struct _ICM20608g{
	int16_t acc_x_raw;
	int16_t acc_y_raw;
	int16_t acc_z_raw;
	int16_t temperature_raw;
	int16_t gyro_x_raw;
	int16_t gyro_y_raw;
	int16_t gyro_z_raw;

	float acc_x;
	float acc_y;
	float acc_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
}Struct_ICM20608g;

/**
 * @brief ICM20608g structure definition.
 */

//extern Struct_ICM20608g ICM20608g;


/**
 * @brief ICM20608g function prototype definition.
 */
void ICM20608g_Writebyte(uint8_t reg_addr, uint8_t val);
uint8_t ICM20608g_Readbyte(uint8_t reg_addr);
void ICM20608g_Readbytes(uint8_t reg_addr, uint8_t len, uint8_t* data);
void ICM20608g_I2C_Initialization(void);
void ICM20608g_GPIO_I2C_Initialization(void);
int ICM20608g_Initialization(void);
void ICM20608g_Get6AxisRawData(int16_t* accel, int16_t* gyro);
void ICM20608g_Get6Data( Struct_ICM20608g *ICM20608g );
void ICM20608g_Get3AxisGyroRawData(uint16_t* gyro);
void ICM20608g_Get3AxisAccRawData(uint16_t* accel);
//int ICM20608g_DataReady(void);

#endif /* ICM20608g_ICM20608g_H_ */
