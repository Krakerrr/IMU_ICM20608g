/*
 * ICM20608g.c
 *
 *  Created on: 15 Ağu 2023
 *      Author: KRAKER
 */


#include "ICM20608g.h"

/*
 * I2C init
 */
void ICM20608g_I2C_Initialization(void)
{
	hICM20608g_I2C.Instance 			= ICM20608g_I2C_CHANNEL;
	hICM20608g_I2C.Init.ClockSpeed		= 400000;
	hICM20608g_I2C.Init.DutyCycle 		= I2C_DUTYCYCLE_2;
	hICM20608g_I2C.Init.OwnAddress1 	= 0;
	hICM20608g_I2C.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
	hICM20608g_I2C.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hICM20608g_I2C.Init.OwnAddress2		= 0;
	hICM20608g_I2C.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hICM20608g_I2C.Init.NoStretchMode 	= I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hICM20608g_I2C) != HAL_OK)
	{
		UARTprintmsg("ICM20608g_I2C_Initialization error \r\n");
		Error_Handler();
	}
}

/*
 * I2C gpio INIT
 */

void ICM20608g_GPIO_I2C_Initialization(I2C_HandleTypeDef* hi2c)
{
	if(hi2c->Instance == ICM20608g_I2C_CHANNEL)
	{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		/* Peripheral clock enable */
		__ICM20608g_CLK_ENABLE();

		/* I2C1 GPIO Configuration  */
		GPIO_InitStruct.Pin 		= ICM20608g_I2C_SCL_PIN | ICM20608g_I2C_SDA_PIN;
		GPIO_InitStruct.Mode		= GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull 		= GPIO_PULLUP;
		GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate 	= ICM20608g_GPIO_AF;
		HAL_GPIO_Init(ICM20608g_I2C_SCL_PORT, &GPIO_InitStruct);


		/*	NVIC */
		//	__ICM20608g_NVIC_ENABLE();
	}
}


int ICM20608g_Initialization(void)
{

	uint8_t who_am_i = 0;

	ICM20608g_I2C_Initialization();

	UARTprintmsg("Checking ICM20608g...\r\n");

	HAL_Delay(50);
	who_am_i = ICM20608g_Readbyte(WHO_AM_I);

	// who am i = 0xAF
	if(who_am_i == 0xAF)
	{
		UARTprintmsg("\nICM20608g who_am_i = 0x%02x...OK\r\n", who_am_i);
	}
	// recheck
	else if(who_am_i != 0x12)
	{
		who_am_i = ICM20608g_Readbyte(WHO_AM_I); // check again WHO_AM_I (0x75)

		if (who_am_i != 0x12){
			UARTprintmsg( "ICM20608g Not OK: 0x%02x Should be 0x%02x\r\n", who_am_i, 0xAF);
			return 1; //ERROR
		}
	}

	// Reset ICM20608g
	// PWR_MGMT_1 0x6B
	ICM20608g_Writebyte(PWR_MGMT_1, 0x80); //Reset ICM20608g
	HAL_Delay(50);

	// PWR_MGMT_1 0x6B // Enable Temperature sensor(bit4-0) do nthing
	ICM20608g_Writebyte(PWR_MGMT_1, 0x01); // Use PLL(bit2:0-01)
	HAL_Delay(50);


	// PWR_MGMT_2 0x6C
	//	ICM20608g_Writebyte(PWR_MGMT_2, 0x38); // Disable Acc(bit5:3-111), Enable Gyro(bit2:0-000)
	ICM20608g_Writebyte( PWR_MGMT_2, 0x00 ); // Enable Acc(bit5:3-000), Enable Gyro(bit2:0-000)
	HAL_Delay(50);

	// set sample rate to 1000Hz and apply a software filter
	ICM20608g_Writebyte(SMPLRT_DIV, 0x00);
	HAL_Delay(50);

	// Gyro DLPF Config
	//ICM20608g_Writebyte(CONFIG, 0x00); // Gyro LPF fc 250Hz(bit2:0-000)
	ICM20608g_Writebyte(CONFIG, 0x04); // Gyro LPF fc 20Hz(bit2:0-100) at 1kHz sample rate
	HAL_Delay(50);

	// GYRO_CONFIG 0x1B
	ICM20608g_Writebyte(GYRO_CONFIG, (0x03 << 3) ); // Gyro sensitivity 2000 dps(bit4:3-11), FCHOICE (bit1:0-00)
	HAL_Delay(50);

	// ACCEL_CONFIG 0x1C
	ICM20608g_Writebyte(ACCEL_CONFIG, (0x03 << 3) ); // Acc sensitivity 16g
	HAL_Delay(50);

	// ACCEL_CONFIG2 0x1D
	ICM20608g_Writebyte(ACCEL_CONFIG2, 0x03); // Acc FCHOICE 1kHz(bit3-0), DLPF fc 44.8Hz(bit2:0-011)
	HAL_Delay(50);

	return 0; //OK
}

///////// I2C functions /////////////
void I2CSendByte(uint8_t datasend)
{
	//TODO: REMOVE ERRORHANDLERS
	//TODO: arrange delay
	if( HAL_I2C_Master_Transmit(&hICM20608g_I2C, ICM20608g_I2C_ADDRESS, &datasend, sizeof(datasend), HAL_MAX_DELAY)!= HAL_OK )
	{
		UARTprintmsg("I2CSendByte error \r\n");
		Error_Handler();
	}
}

void I2CSendBytes(uint8_t* pData, uint16_t datasize)
{
	//TODO: REMOVE ERRORHANDLERS
	//TODO: arrange delay
	if( HAL_I2C_Master_Transmit(&hICM20608g_I2C, ICM20608g_I2C_ADDRESS, pData, datasize, HAL_MAX_DELAY)!= HAL_OK )
	{
		UARTprintmsg("I2CSendBytes error \r\n");
		Error_Handler();
	}
}


uint8_t I2CReadByte(void)
{
	uint8_t val;
	//TODO: REMOVE ERRORHANDLERS
	if( HAL_I2C_Master_Receive(&hICM20608g_I2C, ICM20608g_I2C_ADDRESS, &val, 1, HAL_MAX_DELAY) != HAL_OK )
	{
		UARTprintmsg("I2CReadByte error \r\n");
		Error_Handler();
	}

	return val;
}

void I2CReadBytes( uint8_t* recvdata, uint8_t recvdatasize)
{
	//TODO: REMOVE ERRORHANDLERS
	if( HAL_I2C_Master_Receive(&hICM20608g_I2C, ICM20608g_I2C_ADDRESS, recvdata, recvdatasize, HAL_MAX_DELAY) != HAL_OK )
	{
		UARTprintmsg("I2CReadBytes error \r\n");
		Error_Handler();
	}

}



//////////////////////////////////////////////////////////////

uint8_t ICM20608g_Readbyte(uint8_t reg_addr)
{
	I2CSendByte( reg_addr );

	return I2CReadByte();
}


void ICM20608g_Writebyte(uint8_t reg_addr, uint8_t val)
{
	uint8_t sendata[2] = {reg_addr,val};

	I2CSendBytes(sendata, 2);
}

void ICM20608g_Readbytes(uint8_t reg_addr, uint8_t len, uint8_t* data)
{
	I2CSendByte( reg_addr );

	I2CReadBytes( data, len);
}


//////////////////////////////////////////////

void ICM20608g_Get3AxisGyroRawData(uint16_t* gyro)
{
	uint8_t data[6];
	ICM20608g_Readbytes(GYRO_XOUT_H, 6, data);

	gyro[0] = ((data[0] << 8) | data[1]);
	gyro[1] = ((data[2] << 8) | data[3]);
	gyro[2] = ((data[4] << 8) | data[5]);
}


void ICM20608g_Get3AxisAccRawData(uint16_t* accel)
{
	uint8_t data[6];
	ICM20608g_Readbytes(ACCEL_XOUT_H, 6, data);

	accel[0] = ((data[0] << 8) | data[1]);
	accel[1] = ((data[2] << 8) | data[3]);
	accel[2] = ((data[4] << 8) | data[5]);
}



void ICM20608g_Get6AxisRawData(int16_t* accel, int16_t* gyro)
{
	uint8_t data[14];
	ICM20608g_Readbytes(ACCEL_XOUT_H, 14, data);

	accel[0] = (data[0] << 8) | data[1];
	accel[1] = (data[2] << 8) | data[3];
	accel[2] = (data[4] << 8) | data[5];

	gyro[0] = ((data[8] << 8) | data[9]);
	gyro[1] = ((data[10] << 8) | data[11]);
	gyro[2] = ((data[12] << 8) | data[13]);
}


void ICM20608g_Get6Data( Struct_ICM20608g *ICM20608g )
{
	const float gyro_lsb_to_degs = 0.06103515625f;	// 2000.f / 32768.f;
	const float accel_lsb_to_g   = 0.00048828125f; 	//16.f / 32768.f;

	uint8_t data[14] = {0};
	ICM20608g_Readbytes(ACCEL_XOUT_H, 14, data);

	ICM20608g->acc_x_raw  = (data[0] << 8) | data[1];
	ICM20608g->acc_y_raw  = (data[2] << 8) | data[3];
	ICM20608g->acc_z_raw  = (data[4] << 8) | data[5];

	ICM20608g->temperature_raw  = (data[6] << 8) | data[7];

	ICM20608g->gyro_x_raw = ((data[8] << 8) | data[9]);
	ICM20608g->gyro_y_raw = ((data[10] << 8) | data[11]);
	ICM20608g->gyro_z_raw = ((data[12] << 8) | data[13]);

	ICM20608g->acc_x  = ( (float) ICM20608g->acc_x_raw ) * accel_lsb_to_g;
	ICM20608g->acc_y  = ( (float) ICM20608g->acc_y_raw ) * accel_lsb_to_g;
	ICM20608g->acc_z  = ( (float) ICM20608g->acc_z_raw ) * accel_lsb_to_g;
	ICM20608g->gyro_x = ( (float) ICM20608g->gyro_x_raw ) * gyro_lsb_to_degs;
	ICM20608g->gyro_y = ( (float) ICM20608g->gyro_y_raw ) * gyro_lsb_to_degs;
	ICM20608g->gyro_z = ( (float) ICM20608g->gyro_z_raw ) * gyro_lsb_to_degs;
}

