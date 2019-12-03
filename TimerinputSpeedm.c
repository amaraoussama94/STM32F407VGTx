#include "stm32f4xx.h"
#include <stdio.h>

char  text[1000];
int  CCR_VALUE,N;
float freq ;
/*********************************************************************************
USART3 configuration :
*********************************************************************************/

void UsartInit()
{

  RCC->AHB1ENR|= 0x00000002; // Enable clock for GPIOB
  RCC->APB1ENR|=0x00040000;   // Enable clock for USART3
  GPIOB->AFR[1]=0x00007700;  // //enable USART3_TX to PB10 and USART3_RX to PB11
  GPIOB->MODER|=0x2AA00000;  // configuring the USART3 ALTERNATE function  to PB10 and PB11
  USART3->BRR=1666;    // 9600 Baud  f=16MHZ 16Mhz/9600=1666
  USART3->CR1|=0x0000202C;  // USART3 enable

}



/************************************************************************************************
//To send data, we simply assign it to the DR register. Then we wait until the TXE (TX empty) flag
//in the SR register is set to be sure the data is transmitted. In the code it looks like this:
 ************************************************************************************************/

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

/**************************************************************/

void config_TIM3_PMW()
{

   RCC->APB1ENR|= 0x2; //clock enable for TIM3
   RCC->AHB1ENR|= 0x1; //clock enable GPIOA
   GPIOA->MODER |= 0x2000; //Alternative function mode PA6
   TIM3->CCMR1 = 0x60; //PWM mode 1 on TIM3 Channel 1
   TIM3->PSC = 500;
   TIM3->ARR =200;
   TIM3->CCR1 =100 ;//valeur de  comparraison avec le ARR
   GPIOA->AFR[0]= 0x2000000; //set GPIOA to AF2
   TIM3->CCER=0x01;//enable  mode   comparison
   TIM3->EGR=0x02;//Configuration du registre EGR pour l’initialisation du compteur et de la mise à jour des registres
   TIM3->CR1 = 0x1; //enable timer 3
}
/********************************Capteur vitesse*************************************/
void config_TIM2_Capture()
{//PA0 chaine 1

	// enable TIM2 clock
	RCC->APB1ENR|= 0x1; //0x00000001
	// configure TIM2_CH1 for input capture //////////////////////////////////
	TIM2->CCMR1 = 0x0101;// set TIM2_CH1 for input mode // set the input filter to 4 samples
	// select edge of the active transition
	//enable input capture
	TIM2->CCER = 0x1;//falling edge
	TIM2->PSC =160;
	TIM2->ARR =2000;
	// enable GPIOA clock
	//RCC->AHB1ENR = 0x00000001;
	// Alternate Function
	GPIOA->MODER|=0x00000002; // AF, TIM2_CH1 // AF, TIM2_CH2
	// Alternate Function pins
	 GPIOA->AFR[0]|=0x1;// TIM2_CH1, AF1 // TIM2_CH2, AF1
	 TIM2->DIER=0x0002; //Enable interrupt for CC1 and CC2
	 NVIC_EnableIRQ(TIM2_IRQn); // Enable TIM2 IRQ
	 TIM2->SR=0;
	 //enable TIM2
	 TIM2->CR1 = 0x0001;
}
/***********************************************************************/
void TIM6_config() //T=1s
{
    RCC->APB1ENR |= 0x00000010;   // Enable TIM6 clock
    TIM6->PSC = 16000;                    // Set prescaler to 41999
    TIM6->ARR = 1000;                     // Set auto-reload to 5999
    TIM6->EGR |= 0x01;              // Force update
    TIM6->SR = 0x0000;              // Clear the update flag
    TIM6->DIER |= 0x0001;           // Enable interrupt on update event
    NVIC_EnableIRQ(TIM6_DAC_IRQn);        // Enable TIM6 IRQ
    TIM6->CR1 |= 0x0001;             // Enable TIM6 counter
}

/************************************************************************/
void Affichage_speed()
{
	sprintf (text,"%d",CCR_VALUE);
		  SendTxt(text);
		  SendTxt("\n");
}
/******************************************/
int main()
{
	UsartInit();
	SendTxt("TEST");
    SendTxt("\n");
	config_TIM3_PMW();
	config_TIM2_Capture();
	TIM6_config();

while(1) {	}

}
void TIM6_DAC_IRQHandler()
{
	if((TIM6->SR &0x0001)!=0)
	{
       freq=100000/CCR_VALUE;
		sprintf(text,"%f",freq);
			 SendTxt(text);
			 SendTxt("\n");
				      /*  SendTxt("freq  :  ");
				         freq=1000/N ;
				         sprintf(text,"%f",freq);
				          SendTxt(text);
				         SendTxt("\n");*/
		}
		N=0;
		TIM6->SR&=0;
		}


void TIM2_IRQHandler()
{
  if((TIM2->SR&0x0002)!=0 )
  {
	 CCR_VALUE=TIM2->CCR1;
	 TIM2->CNT=0;
   }
  TIM2->SR&=0;
}


