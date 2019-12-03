#include "stm32f4xx.h"
#include <stdio.h>
int t1=6;

///*****USART6 Config*******///
/*  delay function*/

void Delay(uint32_t time){
	while(time--);
}
//*******************************/
void USART6_config()
{

  RCC->AHB1ENR |= 1 << 2; // Enable clock for GPIOC
  RCC->APB2ENR |= 1 << 5;   // Enable clock for USART3
  GPIOC->AFR[0]|= (0x08 << 24)  | (0x08 << 28) ;// set USART6_TX to PC6 & USART6_RX to PC7
  GPIOC->MODER |= (0x02 << 12) | (0x02 << 14);  // alternatif function at PC6 & PC7
  USART6->BRR = 138;    // 16Mhz/115200 Baud=183
  USART6->CR1|= 0x0000202C;  // USART6 enable


}
/*
void configuration_USART()//USART3 ici  c  est uart pas  usart puuisque  on a   pas  besoin de  clocks
{

  RCC->AHB1ENR |= 0x00000002; // Enable clock for GPIOB
  RCC->APB1ENR|=0x00040000;   // Enable clock for USART3
  GPIOB->AFR[1]=0x00007700;  // //enable USART3_TX to PB10 and USART3_RX to PB11
  GPIOB->MODER|=0x2AA00000;  // configuring the USART3 ALTERNATE function  to PB10 and PB11  alternative  fonctionn  c est output  a haute speed
  USART3->BRR=138;    //BRR= 9600 Baud 42/BRR attenstion au freq

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
**/
void sendChar(USART_TypeDef *USARTx , char c){
	while(!(USARTx->SR & USART_SR_TXE));  // wait for TXBUFF empty
	USARTx->DR = c;
}


void SendText(USART_TypeDef *USARTx , char* txt){
	while(*txt){
		sendChar(USARTx , *txt);
		txt++;
	}
}

void ESP8266_Connect(){

	SendText(USART6,"AT+GMR\r\n");//  chek if the device  is  working
	Delay(600);
    SendText(USART6,"AT+RST\r\n");// reset device
	SendText(USART6,"AT+CWMODE=1\r\n");// choix de mode 1 emission en peut  aussi reception ou roteur
	Delay(600000);
	SendText(USART6,"AT+CWJAP=\"oussama@\",\"25576469\"\r\n ");//wifi name+passe

	Delay(7000000);

}




void Send_Data_To_EPS8266()
{
	char txt2[1000];
	SendText(USART6, "AT+CIPMUX=0\n"); // type d'accées 1 seul  chaine
	Delay(600000);
	SendText(USART6,"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");//  adresse IP
	Delay(7000000);
	SendText( USART6,"AT+CIPSEND=512\r\n"); // data length// debit  de  transfert//
	Delay(2000000);
	sprintf(txt2,"GET /update?api_key=Y2AUYTKEK7F6GU9A&field1=%d\n",t1);
	SendText( USART6,txt2);
	SendText(USART6,"AT+CIPCLOSE\n"); // end comunication
	Delay(6000000);
}


int main (void)
{
  //configuration_USART();
   USART6_config();
   SendText(USART6,"AT \r\n");
     ESP8266_Connect();
   while(1){
    Send_Data_To_EPS8266( );

   }
}
