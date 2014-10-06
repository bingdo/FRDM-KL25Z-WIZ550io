/****************************************************************************************************/
/**
Copyright (c) 2011 Freescale Semiconductor
Freescale Confidential Proprietary
\file       twr_kl25_gpio.c
\brief      
\author     
\author     
\version    1.0
\date       Sep 26, 2011
*/


#include "freedom_gpio.h"
#include "common.h"
#include "Timer.h"

/*
Initialize GPIO of TWRK40
   LEDS as outputs and simple test

SW3 (IRQ0) PTC5 
SW4 (IRQ1) PTC13 
SW5 (RESET) RESET_b 



*/

void  gpio_init(void)
{
  int i;

  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK );

/*
 PORTB_PCR18 = PCR_OUTPUT_CONFIG;
 PORTB_PCR19 = PCR_OUTPUT_CONFIG;
 GPIOB_PDDR |= (1<<18) | (1<<19);
 */
  
#ifdef LED1
  LED1_PCR = PCR_OUTPUT_CONFIG;
  LED1_OFF;
  LED1_OUTPUT_EN;
#endif
    
#ifdef LED2
  LED2_PCR = PCR_OUTPUT_CONFIG;
  LED2_OFF;
  LED2_OUTPUT_EN;
#endif

#ifdef LED3
  LED3_PCR = PCR_OUTPUT_CONFIG;
  LED3_OFF;
  LED3_OUTPUT_EN;
#endif
  
#ifdef LED4
  LED4_PCR = PCR_OUTPUT_CONFIG;
  LED4_OFF;
  LED4_OUTPUT_EN;
#endif
  
#ifdef SW1
   SW1_PCR = PCR_INPUT_CONFIG;
   SW1_INPUT_EN;
#endif
  
#ifdef SW2
   SW2_PCR = PCR_INPUT_CONFIG;
   SW2_INPUT_EN;
#endif

#ifdef SCS
  SCS_PCR = PCR_OUTPUT_CONFIG;
  SCS_OFF;
  SCS_OUTPUT_EN;
#endif

#ifdef RST
  RST_PCR = PCR_OUTPUT_CONFIG;
  RST_OFF;
  RST_OUTPUT_EN;
#endif
 
}


/* 
 raising(input,aux_mem 
  return 1
     when input=1 and aux=0
     aux=input
*/

char input_rise(char input,char *mem)
{
 char result=0;
 if (input && !*mem) result=1;
 *mem = input;
 return result;
}

/* 

  return 1
     when input=1 and aux=0
     aux=input
*/

char input_fall(char input,char *mem)
{
 char result=0;
 if (!input && *mem) result=1;
 *mem = input;
 return result;
}
