
/*********************************************************************************************
* File name	: user_SaveOption.c
* HardWare	: LPC1788 and eeprom_st24c02 
* Author		: Lius
* History		:	none
*	Data			:	2014-07-17
* Descript	: 保存程序配置
*	Version		: V1.0.0
*********************************************************************************************/

/********************************************************************************************
*			include files
********************************************************************************************/
#include "user_SaveOption.h"
#include "stdio.h"

/*****************************************************************************
 * Defines and typedefs
 ****************************************************************************/
 #define DATA_SIZE		(34)
 #define PAGE_SIZE		(8)
 #define DATA_ADRESS	(0x20)
 #define RATI_ADRESS	(0x40)
 #define RATI_SIZE		(0x80)
 #define MAST_ADRESS	(0xC1)
 #define MAST_SIZE		(0x04)
 
void INT32TOUINT8(uint8_t data[],int32_t fp1,int32_t fp2);
void UINT8TOINT32(uint8_t data[],int32_t fp1[],int32_t fp2[]);
Bool EEPROM_ReadArray(uint8_t data[],uint8_t size);
Bool EEPROM_WriteArray(uint8_t data[],uint8_t size);

/*********************************************************************************************
* name			:		EEPROM_WriteStruct
* func			:		EEPROM写结构体
* para			:		EEPROMDataStruct savestruct
* ret				:		none
* comment		:	
*********************************************************************************************/
Bool EEPROM_WriteStruct(EEPROMDataStruct savestruct)
{
	uint8_t WriteData[8];
	uint8_t i;
	uint8_t num;
	
	num=0;
	
	
	for(i=0;i<4;i++)
	{
		WriteData[num]=savestruct.ipConfig.ip[i];
		num+=1;
	}
	
	for(i=0;i<4;i++)
	{
		WriteData[num]=savestruct.ipConfig.gw[i];
		num+=1;
	}
	
	if(EEPROM_WriteDataOnePage(WriteData,PAGE_SIZE,DATA_ADRESS)==FALSE)
		return  FALSE;
	OSTimeDly(6);
	
	num=0;
	for(i=0;i<4;i++)
	{
		WriteData[num]=savestruct.ipConfig.sub[i];
		num+=1;
	}
	
	for(i=0;i<4;i++)
	{
		WriteData[num]=savestruct.ipConfig.mac[i];
		num+=1;
	}
	if(EEPROM_WriteDataOnePage(WriteData,PAGE_SIZE,(DATA_ADRESS+PAGE_SIZE*1)&0xff)==FALSE)
		return  FALSE;
	num=0;
	OSTimeDly(6);
	
	for(i=0;i<2;i++)
	{
		WriteData[num]=savestruct.ipConfig.mac[i+4];
		num+=1;
	}
	
	for(i=0;i<6;i++)
	{
		WriteData[num]=savestruct.uartConfig[i];
		num+=1;
	}
	if(EEPROM_WriteDataOnePage(WriteData,PAGE_SIZE,(DATA_ADRESS+PAGE_SIZE*2)&0xff)==FALSE)
		return  FALSE;
	num=0;
	OSTimeDly(6);
	
	for(i=0;i<6;i++)
	{
		WriteData[num]=savestruct.password[i];
		num+=1;
	}
	
	if(EEPROM_WriteDataOnePage(WriteData,6,(DATA_ADRESS+PAGE_SIZE*3)&0xff)==FALSE)
		return  FALSE;
	OSTimeDly(6);
	
	if(EEPROM_WriteDataOnePage(savestruct.masterIP,MAST_SIZE,(MAST_ADRESS)&0xff)==FALSE)
		return  FALSE;
	
	OSTimeDly(6);
	
	return TRUE;
}

/*********************************************************************************************
* name			:		EEPROM_ReadArray
* func			:		EEPROM读数组
* para			:		none
* ret				:		uint8_t data[]
* comment		:	
*********************************************************************************************/
Bool EEPROM_ReadArray(uint8_t data[],uint8_t size)
{
	if(EEPROM_ReadData(data,size,DATA_ADRESS)==FALSE)
	{
		return FALSE;
	}
}

/*********************************************************************************************
* name			:		EEPROM_WriteArray
* func			:		EEPROM读数组
* para			:		none
* ret				:		uint8_t data[],uint8_t size
* comment		:	
*********************************************************************************************/
Bool EEPROM_WriteArray(uint8_t data[],uint8_t size)
{
	uint8_t i;
	uint8_t num,times;
	uint8_t writeData[8];
	
	num=0;
	times=0;
	for(i=0;i<size;i++)
	{
		if(num>=8)
		{
			num=0;
			
			if(EEPROM_WriteDataOnePage(writeData,8,(DATA_ADRESS+PAGE_SIZE*times)&0xff)==FALSE)
				return  FALSE;
			times+=1;
			OSTimeDly(6);
		}	
		writeData[num]=data[i];
	}
	if(EEPROM_WriteDataOnePage(writeData,size-times*8,(DATA_ADRESS+PAGE_SIZE*times)&0xff)==FALSE)
				return  FALSE;
	return TRUE;
}

/*********************************************************************************************
* name			:		EEPROM_ReadStruct
* func			:		EEPROM读结构体
* para			:		none
* ret				:		EEPROMDataStruct savestruct
* comment		:	
*********************************************************************************************/
EEPROMDataStruct EEPROM_ReadStruct()
{
	EEPROMDataStruct savestruct;
	uint8_t ReadData[DATA_SIZE];
	uint8_t i;
	uint8_t num;
	
	num=0;
	
	
	if(EEPROM_ReadData(ReadData,DATA_SIZE,DATA_ADRESS)==FALSE)
	{
	}
	
	for(i=0;i<4;i++)
	{
		savestruct.ipConfig.ip[i]=ReadData[num];
		num+=1;
	}
	
	for(i=0;i<4;i++)
	{
		savestruct.ipConfig.gw[i]=ReadData[num];
		num+=1;
	}
	
	for(i=0;i<4;i++)
	{
		savestruct.ipConfig.sub[i]=ReadData[num];
		num+=1;
	}
	
	for(i=0;i<6;i++)
	{
		savestruct.ipConfig.mac[i]=ReadData[num];
		num+=1;
	}
	
	for(i=0;i<6;i++)
	{
		savestruct.uartConfig[i]=ReadData[num];
		num+=1;
	}
	
	for(i=0;i<6;i++)
	{
		savestruct.password[i]=ReadData[num];
		num+=1;
	}
	
	for(i=0;i<4;i++)
	{
		savestruct.masterIP[i]=ReadData[num];
		num+=1;
	}
	
	if(EEPROM_ReadData(savestruct.masterIP,MAST_SIZE,MAST_ADRESS)==FALSE)
	{
	}
	
	return savestruct;
}

/*********************************************************************************************
* name			:		EEPROM_WriteRati
* func			:		EEPROM写参数配置结构体
* para			:		Correct_Input savestruct
* ret				:		none
* comment		:	
*********************************************************************************************/
Bool EEPROM_WriteRati(Correct_Input savestruct)
{
	uint8_t WriteData[PAGE_SIZE];
	uint8_t i;

	for(i=0;i<8;i++)
	{
		INT32TOUINT8(WriteData,savestruct.intercept[2*i],savestruct.intercept[2*i+1]);
		if(EEPROM_WriteDataOnePage(WriteData,PAGE_SIZE,(RATI_ADRESS+PAGE_SIZE*i)&0xff)==FALSE)
		{
			printf("Read Rati Failed in roll 1 num %d\n",i+1);
			return  FALSE;
		}
		OSTimeDly(6);
	}
	for(i=0;i<8;i++)
	{
		INT32TOUINT8(WriteData,savestruct.slope[2*i],savestruct.slope[2*i+1]);
		if(EEPROM_WriteDataOnePage(WriteData,PAGE_SIZE,(RATI_ADRESS+0x40+PAGE_SIZE*i)&0xff)==FALSE)
		{
			printf("Read Rati Failed in roll 2 num %d\n",i+1);
			return  FALSE;
		}
		OSTimeDly(6);
	}
	printf("Save Rati Success!\n");
	return  TRUE;
}
/*********************************************************************************************
* name			:		EEPROM_ReadRati
* func			:		EEPROM读参数配置结构体
* para			:		none
* ret				:		Correct_Input
* comment		:	
*********************************************************************************************/
Correct_Input EEPROM_ReadRati()
{
	Correct_Input savestruct;
	uint8_t ReadData[RATI_SIZE];
	uint8_t i;
	
	if(EEPROM_ReadData(ReadData,RATI_SIZE,RATI_ADRESS)==FALSE)
	{
		printf("Read Rati Failed!\n");
	}
	UINT8TOINT32(ReadData,savestruct.intercept,savestruct.slope);
	printf("Read Rati Success!\n");
	return savestruct;
}

/*********************************************************************************************
* name			:		INT32TOUINT8
* func			:		INT32转UINT8
* para			:		uint8_t data[],int32_t fp1,int32_t fp2
* ret				:		none
* comment		:	
*********************************************************************************************/
void INT32TOUINT8(uint8_t data[],int32_t fp1,int32_t fp2)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		if(i<4)
		{
			data[i]=(fp1&(0xF000>>(i*4)))>>((3-i)*4);
		}
		else
		{
			data[i]=(fp2&(0xF000>>((i-4)*4)))>>((7-i)*4);
		}
	}
}

/*********************************************************************************************
* name			:		UINT8TOFP32
* func			:		UINT8转INT32
* para			:		uint8_t data[],int32_t fp1[],int32_t fp2[]
* ret				:		none
* comment		:	
*********************************************************************************************/
void UINT8TOINT32(uint8_t data[],int32_t fp1[],int32_t fp2[])
{
	uint8_t i;
	uint8_t dataSize;
	dataSize=0x40;
	for(i=0;i<16;i++)
	{
		fp1[i]=(data[4*i]<<(3*4))|(data[4*i+1]<<(2*4))|(data[4*i+2]<<(1*4))|(data[4*i+3]);
	}
	for(i=0;i<16;i++)
	{
		fp2[i]=(data[4*i+dataSize]<<(3*4))|(data[4*i+1+dataSize]<<(2*4))|(data[4*i+2+dataSize]<<(1*4))|(data[4*i+3+dataSize]);
	}
}

