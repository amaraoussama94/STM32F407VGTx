
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

int ADC_VALUE ;


/******************Timer ouput******/
void Config_PWM(void)
{

   RCC->APB1ENR |= 0x2; //clock enable for TIM3
   RCC->AHB1ENR |= 0x1; //clock enable GPIOA
   GPIOA->MODER |= 0x2000; //Alternative function mode PA6
   TIM3->CCMR1 = 0x60; //PWM mode 1 on TIM3 Channel 1
   TIM3->PSC = 1000;
   TIM3->ARR =200;
   TIM3->CCR1 = 0;//valeur de  comparraison avec le ARR
   GPIOA->AFR[0] = 0x2000000; //set GPIOA to AF2
   TIM3->CCER=0x01;//enable  mode   comparison
   TIM3->EGR=0x02;//Configuration du registre EGR pour l’initialisation du compteur et de la mise à jour des registres
   TIM3->CR1 = 0x1; //enable timer 3
}

/*******************ADC*******speed-controle***********/
void configuration_ADC()//la modifier
{
	RCC->APB2ENR |= 0x00000100 ; // Enable clock for ADC1
	RCC->AHB1ENR |= 0x00000001 ; // Enable clock for GPIOA
	ADC->CCR = 0x00000006; // No DMA, Regular simultaneous mode only
	ADC1->CR2 = 0x00000001; // Control Register 2: ADC1 ON
	ADC1->SQR3 = 0x00000002; // regular SeQuence Register //5bit  donne l orde  des  chaine de ADC
	GPIOA->MODER |= 0x00000030; // MODE Register -> PA2  analog inputs
}

void conversion_ADC()
{
	ADC1->CR2 |= ADC_CR2_SWSTART; // simultaneous Start Conversion
    while((ADC1->SR & 0x2)==0); // wait for ADC1 conversion to complete 0  not compl++ 1 compl
    ADC_VALUE=ADC1->DR;  //READ RESULT CONVERSION

}

/******************************************************************/
int main(void)
{

  Config_PWM();
  configuration_ADC();

  while(1)
  {
	  conversion_ADC();

	  TIM3->CCR1=ADC_VALUE*0.05;//1-3500/4095=pas  2 pas*3000=max  3 ARR/max=0.05

 }
}
