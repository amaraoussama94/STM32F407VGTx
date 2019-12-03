
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include "misc.h"

char text[60];
float Data_capteur=0;
uint8_t Data_RTC_year,Data_RTC_Month,Data_RTC_Day;
uint8_t  Data_RTC_Date,Data_RTC_Hours,Data_RTC_Munites,Data_RTC_second;
//*****I2C*****//
typedef enum {ERROR1 = 0, SUCCESS1 = !ERROR} ErrorStatus1;
ErrorStatus1 status = ERROR1;
uint32_t lastevent = 0;
uint32_t flag1 = 0, flag2 = 0;


/**********************I2C**************************/
/*************************ChekFlags*******************/
ErrorStatus I2C_CheckEvent1() //CHECK SB FLAG
{
  flag1 = I2C1->SR1;
  flag2 = I2C1->SR2;
  flag2 = flag2 << 16;
/* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2);
/* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & 0x00030001) == 0x00030001)
   {
     /* SUCCESS: last event is equal to I2C_EVENT */
     status = SUCCESS1;
   }
  else
   {
     /* ERROR: last event is different from I2C_EVENT */
    status = ERROR1;
    }
/* Return status */
  return status;
}

ErrorStatus I2C_CheckEvent2() //CHECK ADRR FLAG MODE WRITE
 {
  flag1 = I2C1->SR1;
  flag2 = I2C1->SR2;
  flag2 = flag2 << 16;
  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2);
  /* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & 0x00070082) == 0x00070082)
    {
     /* SUCCESS: last event is equal to I2C_EVENT */
     status = SUCCESS1;
    }
  else
   {
    /* ERROR: last event is different from I2C_EVENT */
    status = ERROR1;
   }
  /* Return status */
  return status;
}

ErrorStatus I2C_CheckEvent3() //CHECK TXE FLAG
{
  flag1 = I2C1->SR1;
  flag2 = I2C1->SR2;
  flag2 = flag2 << 16;
  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2);
  /* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & 0x00070080) == 0x00070080)
   {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS1;
   }
  else
   {
    /* ERROR: last event is different from I2C_EVENT */
     status = ERROR1;
   }
  /* Return status */
  return status;
}

ErrorStatus I2C_CheckEvent4() //CHECK ADRR FLAG MODE READ
{
  flag1 = I2C1->SR1;
  flag2 = I2C1->SR2;
  flag2 = flag2 << 16;
  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2);
  /* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & 0x00030002) == 0x00030002)
   {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS1;
   }
  else
   {
     /* ERROR: last event is different from I2C_EVENT */
     status = ERROR1;
    }
  /* Return status */
  return status;
  }

ErrorStatus I2C_CheckEvent5() // CHECK RXNE
{
  flag1 = I2C1->SR1;
  flag2 = I2C1->SR2;
  flag2 = flag2 << 16;
  /* Get the last event value from I2C status register */
  lastevent = (flag1 | flag2);
  /* Check whether the last event contains the I2C_EVENT */
  if ((lastevent & 0x00030040) == 0x00030040)
   {
    /* SUCCESS: last event is equal to I2C_EVENT */
    status = SUCCESS1;
   }
  else
   {
    /* ERROR: last event is different from I2C_EVENT */
    status = ERROR1;
   }
  /* Return status */
  return status;
}
/****************************************************************/
void I2C_Capteur_Cond()
{
	  /*-----------------------------------*/
	  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
	  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	  I2C1->DR =((0x48) << 1); /* Send the address */
	  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	  I2C1->DR=0XEE;//start conversation
	  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
}

float  I2C_Capteur()
{/***adress  0b1001000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x48) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
 // I2C1->DR=0XEE;//start conversion
  //while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C1->DR=0XAA;//read T°
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
  //********************************************************
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x48) << 1)|1; /* Send the address */
  while(I2C_CheckEvent4()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); //NACK
  while(I2C_CheckEvent5()==0); //while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  Data_capteur=I2C1->DR;
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
return Data_capteur;
}
void I2C_Capteur_stop()
{
	I2C1->CR1 |= I2C_CR1_START;//I2C_START;
	  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	  I2C1->DR =((0x48) << 1); /* Send the address */
	  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	  I2C1->DR=0X22;//stop conversion°
	  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
}
/********************RTC********************/
uint8_t TM_DS1307_Bcd2Bin(uint8_t bcd) {
	uint8_t dec = 10 * (bcd >> 4);
	dec += bcd & 0x0F;
	return dec;
}

uint8_t TM_DS1307_Bin2Bcd(uint8_t bin) {
	uint8_t low = 0;
	uint8_t high = 0;

	/* High nibble */
	high = bin / 10;
	/* Low nibble */
	low = bin - (high * 10);

	/* Return */
	return high << 4 | low;
}

void I2C_RTC_seconds_init()
{
	I2C1->CR1 |= I2C_CR1_START;//I2C_START;
	  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	  I2C1->DR =((0x68) << 1); /* Send the address */
	  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	  I2C1->DR=0x00;//adress registre
	  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	  I2C1->DR=TM_DS1307_Bin2Bcd(0b00000000) ;//remise  a zero seconds
	  while(I2C_CheckEvent3()==0);
	  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
}
void I2C_RTC_munites_init()
{
	 I2C1->CR1 |= I2C_CR1_START;//I2C_START;
	  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	  I2C1->DR =((0x68) << 1); /* Send the address */
	  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	  I2C1->DR=0x01;
	  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	  I2C1->DR=TM_DS1307_Bin2Bcd(0b00011101);//remize a  zero de munites
	   while(I2C_CheckEvent3()==0);
	  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
}
void I2C_RTC_hours_init()
{
	 I2C1->CR1 |= I2C_CR1_START;//I2C_START;
	  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	  I2C1->DR =((0x68) << 1); /* Send the address */
	  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	  I2C1->DR=0x02;
	  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	  I2C1->DR=TM_DS1307_Bin2Bcd(0b00010000);//rmize  a zero  et selection mode 24H
	  while(I2C_CheckEvent3()==0);
	  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
}

/*----------------------------------------------------------*/
void I2C_RTC_seconds()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x00;//adress registre
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  //I2C1->DR=TM_DS1307_Bin2Bcd(0b00000000) ;//remise  a zero seconds
 // while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
  //********************************************************
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1)|1; /* Send the address */
  while(I2C_CheckEvent4()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); //NACK
  while(I2C_CheckEvent5()==0); //while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  Data_RTC_second=TM_DS1307_Bcd2Bin(I2C1->DR);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);0
return Data_RTC_second;
}

uint8_t I2C_RTC_munites()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x01;
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  //I2C1->DR=TM_DS1307_Bin2Bcd(0b00000000);//remize a  zero de munites
  /// while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
  //********************************************************
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1)|1; /* Send the address */
  while(I2C_CheckEvent4()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); //NACK
  while(I2C_CheckEvent5()==0); //while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  Data_RTC_Munites=TM_DS1307_Bcd2Bin(I2C1->DR);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);0
return   Data_RTC_Munites;
}
uint8_t I2C_RTC_Hours()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x02;
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
//  I2C1->DR=TM_DS1307_Bin2Bcd(0b00000000);//rmize  a zero  et selection mode 24H
//  while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
  //********************************************************
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1)|1; /* Send the address */
  while(I2C_CheckEvent4()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); //NACK
  while(I2C_CheckEvent5()==0); //while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  Data_RTC_Hours=TM_DS1307_Bcd2Bin(I2C1->DR);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);0
return Data_RTC_Hours;
}
/*---------------------------------------------------------------------------*/
uint8_t I2C_RTC_DATE_Init()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x04;
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C1->DR=TM_DS1307_Bin2Bcd(0b00001101);/// a  voir
   while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
}

uint8_t I2C_RTC_Month_Init()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x05;
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
   I2C1->DR=TM_DS1307_Bin2Bcd(0b00001100);//1st month
    while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
}

uint8_t I2C_RTC_year_Init()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x06;
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C1->DR=TM_DS1307_Bin2Bcd(0b00010010);//year2001  0x21
  while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);

}
/*-------------------------------------------------------------*/
uint8_t I2C_RTC_day()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x03;
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  //I2C1->DR=TM_DS1307_Bin2Bcd(0b00001010);//1er jour
 //  while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
  //********************************************************
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1)|1; /* Send the address */
  while(I2C_CheckEvent4()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); //NACK
  while(I2C_CheckEvent5()==0); //while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  Data_RTC_Day=TM_DS1307_Bcd2Bin(I2C1->DR);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);0
return Data_RTC_Day;
}
uint8_t I2C_RTC_Month()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x05;
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
   //I2C1->DR=TM_DS1307_Bin2Bcd(0b00000100);//1st month
  //  while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
  //********************************************************
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1)|1; /* Send the address */
  while(I2C_CheckEvent4()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); //NACK
  while(I2C_CheckEvent5()==0); //while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  Data_RTC_Month=TM_DS1307_Bcd2Bin(I2C1->DR);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);0
return Data_RTC_Month;
}
uint8_t I2C_RTC_year()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x06;
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 // I2C1->DR=TM_DS1307_Bin2Bcd(0b00010000);//year2001  0x21
 // while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
  //********************************************************
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1)|1; /* Send the address */
  while(I2C_CheckEvent4()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); //NACK
  while(I2C_CheckEvent5()==0); //while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  Data_RTC_year=TM_DS1307_Bcd2Bin(I2C1->DR);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);0
return Data_RTC_year;
}
uint8_t I2C_RTC_DATE()
{/***adress RTC  0b1101000*/
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1); /* Send the address */
  while(I2C_CheckEvent2()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->DR=0x04;
  while(I2C_CheckEvent3()==0);// while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 // I2C1->DR=TM_DS1307_Bin2Bcd(0b00001100);/// a  voir
 //  while(I2C_CheckEvent3()==0);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);
  //********************************************************
  I2C1->CR1 |= I2C_CR1_START;//I2C_START;
  while(I2C_CheckEvent1()==0); //while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
  I2C1->DR =((0x68) << 1)|1; /* Send the address */
  while(I2C_CheckEvent4()==0); // while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  I2C1->CR1 &= (uint16_t)~((uint16_t)I2C_CR1_ACK); //NACK
  while(I2C_CheckEvent5()==0); //while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  Data_RTC_Date=TM_DS1307_Bcd2Bin(I2C1->DR);
  I2C1->CR1 |= I2C_CR1_STOP;//I2C_stop(I2C1);0
return Data_RTC_Date;
}
/*****************************/
void Init_I2C()
{
     //configure the AF
   RCC->AHB1ENR |= 1<<1; //Clock for GPIOB
   RCC->APB1ENR |= 1 <<21; //clock to I2C1
   GPIOB->AFR[0] |= (4<< 24); //enable SCK to PB6
   GPIOB->AFR[0] |= (4<< 28); //enable SDA to PB7
   GPIOB->MODER &= ~(3 << 12); //clear bits 12 & 13 (PB6)
   GPIOB->MODER |= 2 << 12; //MODER6[1:0] = 10 bin
   GPIOB->MODER &= ~(3 << 14); //clear bits 14 & 15 (PB7)
   GPIOB->MODER |= 2 << 14; //MODER9[1:0] = 10 bin
   GPIOB->OTYPER |= 1 <<6; //PB6 open drain no pullup no pulldown
   GPIOB->OTYPER |= 1<<7; //PB7 open drain
   GPIOB->PUPDR &= ~(3 << 12);//clear bits 12 & 13 (PB6)
   GPIOB->PUPDR &= ~(3 << 14);//clear bits 14 & 15 (PB7)
   //Configure the I2C
   I2C1->CR2 = 0x0110; //6MHz must inferieur a 16MHZ
   I2C1->CCR = 0x0050; //100kHz Bit rate standar mode
   I2C1->TRISE |= 0x0005; // rise time  0x05----(1000 ns / 250 ns = 4 + 1 = 5)
   I2C1->CR1 = 0x0001; //enable peripheral
}

/***********UsART******************/
void configuration_USART()//USART3 ici  c  est uart pas  usart puuisque  on a   pas  besoin de  clocks 
{

  RCC->AHB1ENR |= 0x00000002; // Enable clock for GPIOB
  RCC->APB1ENR|=0x00040000;   // Enable clock for USART3
  GPIOB->AFR[1]=0x00007700;  // //enable USART3_TX to PB10 and USART3_RX to PB11
  GPIOB->MODER|=0x2AA00000;  // configuring the USART3 ALTERNATE function  to PB10 and PB11  alternative  fonctionn  c est output  a haute speed 
  USART3->BRR=1666;    //BRR= 9600 Baud 42/BRR attenstion au freq

  USART3->CR1|=0x0000202C;  // USART3 enable  enable  transmission a  and   reception  et  les   inettereption  en mode  recption 
  // NVIC->ISER[1]|=0x80;         // NVIC da USART3 interrupta izin verelim
}


void SendChar(char Tx)
{
  while((USART3->SR&0x80)==0);  // On attend à ce que TXBUFF soit vide (libere) ou (FLAG TXNE=1) ou Le caractere est envoyé
  USART3->DR=Tx;
}

void SendTxt(char *Adr)
{
  while(*Adr)
  {
    SendChar(*Adr);
    Adr++;
  }
}
/*****************************************/
void Affichage_Temp()
{             SendTxt("temp:  ");
     sprintf (text,"%f", Data_capteur);
		  	  SendTxt(text);
		  	  SendTxt("\n");

}

void Affichage_T()
{
	 sprintf (text,"clk:   %d:%d:%d ",Data_RTC_Hours,Data_RTC_Munites,Data_RTC_second);
		  	  SendTxt(text);
		  	  SendTxt("\n");

}

void Affichage_TD()
{
	 sprintf (text,"Date:yyyy/mm/dd  20%d/%d/%d ",Data_RTC_year,Data_RTC_Month, Data_RTC_Date);
		  	  SendTxt(text);
		  	  SendTxt("\n");

}
/******************************************************************/
int main(void)
{
	configuration_USART();
	Init_I2C();
	I2C_Capteur_Cond();
	I2C_RTC_seconds_init();
	I2C_RTC_munites_init();
	I2C_RTC_hours_init();
	I2C_RTC_DATE_Init();
	I2C_RTC_Month_Init();
	I2C_RTC_year_Init();
    while(1)
    {
    	I2C_Capteur();
        Affichage_Temp();

       I2C_RTC_seconds();
        I2C_RTC_munites();
        I2C_RTC_Hours();
        Affichage_T();
       I2C_RTC_day();
        I2C_RTC_Month();
        I2C_RTC_year();
       I2C_RTC_DATE();
        Affichage_TD();
    }
}
-
