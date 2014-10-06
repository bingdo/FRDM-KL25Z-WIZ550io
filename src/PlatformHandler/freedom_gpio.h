/****************************************************************************************************/
/**
Copyright (c) 2011 Freescale Semiconductor
Freescale Confidential Proprietary
\file       TWR-K20D540M_DEF.c
\brief      Definitions of Leds and Push button and function to intialize them

Features	Port/peripheral
ACCELE	I2C+GPIO
ELEC1	TSI0_CH9
ELEC2	TSI0_CH10
IR LED 	PTE22
IR Rx	PTE23/ADC/UART
LED Green	PTA7
LED red 	PTB8
LED yellow 	PTA16
LED Orange	PTA5/FTM0_CH2
POT	PTE29/ADC0_SE48
Push Button SW2	PTA4
Push Button SW3	PTC3



\author     
\author     
\version    1.0
\date       Sep 26, 2011
*/

#define BIT_MASK(x)   (0x1<<x)

// User should only define wich port and the bit 


#define LED1_PORT   B   // REd LED
#define LED1_BIT    18

#define LED2_PORT   B   // GREEN LED
#define LED2_BIT    19

//#define LED3_PORT   D
//#define LED3_BIT    1

//#define LED4_PORT   A
//#define LED4_BIT    5

//#define  SW1_PORT   C
//#define  SW1_BIT    9

//#define  SW2_PORT   C
//#define  SW2_BIT    3

#define SCS_PORT   D
#define SCS_BIT    0

#define RST_PORT   A
#define RST_BIT    20

// Compilation time Definitions

#define PCR(PTO,BIT)  PORT##PTO##_PCR##BIT
#define PDDR(PTO)     GPIO##PTO##_PDDR
#define PSOR(PORT)    GPIO##PORT##_PSOR
#define PCOR(PORT)    GPIO##PORT##_PCOR
#define PTOR(PORT)    GPIO##PORT##_PTOR
#define PDOR(PORT)    GPIO##PORT##_PDOR
#define PDIR(PORT)    GPIO##PORT##_PDIR



#define  PCR_OUTPUT_CONFIG  (PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK ) //Pin configured as GPIO
#define  PCR_INPUT_CONFIG   (PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK) //Pin configured as GPIO Input with pull up enable


#ifdef  LED1_PORT
#define LED1
#define LED1_PCR_OUTPUT(LED1_PORT,LED1_BIT)  PCR(LED1_PORT,LED1_BIT)
#define LED1_DDR_OUTPUT(LED1_PORT,LED1_BIT)  PDDR(LED1_PORT) |= (1<<LED1_BIT)
#define LED1_PSOR(LED1_PORT,LED1_BIT)        PSOR(LED1_PORT) |= (1<<LED1_BIT)
#define LED1_PCOR(LED1_PORT,LED1_BIT)        PCOR(LED1_PORT) |= (1<<LED1_BIT)
#define LED1_PTOR(LED1_PORT,LED1_BIT)        PTOR(LED1_PORT) |= (1<<LED1_BIT)

#define LED1_PCR        LED1_PCR_OUTPUT(LED1_PORT,LED1_BIT) 
#define LED1_OUTPUT_EN  LED1_DDR_OUTPUT(LED1_PORT,LED1_BIT) 

#define LED1_SET         LED1_PSOR(LED1_PORT,LED1_BIT)
#define LED1_CLR         LED1_PCOR(LED1_PORT,LED1_BIT)

#define LED1_ON         LED1_PCOR(LED1_PORT,LED1_BIT)
#define LED1_OFF        LED1_PSOR(LED1_PORT,LED1_BIT)
#define LED1_TOGGLE     LED1_PTOR(LED1_PORT,LED1_BIT)
#endif


#ifdef  LED2_PORT
#define LED2
#define LED2_PCR_OUTPUT(LED2_PORT,LED2_BIT)  PCR(LED2_PORT,LED2_BIT)
#define LED2_DDR_OUTPUT(LED2_PORT,LED2_BIT)  PDDR(LED2_PORT) |= (1<<LED2_BIT)
#define LED2_PSOR(LED2_PORT,LED2_BIT)        PSOR(LED2_PORT) |= (1<<LED2_BIT)
#define LED2_PCOR(LED2_PORT,LED2_BIT)        PCOR(LED2_PORT) |= (1<<LED2_BIT)
#define LED2_PTOR(LED2_PORT,LED2_BIT)        PTOR(LED2_PORT) |= (1<<LED2_BIT)

#define LED2_PCR         LED2_PCR_OUTPUT (LED2_PORT,LED2_BIT)
#define LED2_OUTPUT_EN   LED2_DDR_OUTPUT (LED2_PORT,LED2_BIT)

#define LED2_SET      LED2_PSOR(LED2_PORT,LED2_BIT)
#define LED2_CLR      LED2_PCOR(LED2_PORT,LED2_BIT)

#define LED2_ON       LED2_PCOR(LED2_PORT,LED2_BIT)
#define LED2_OFF      LED2_PSOR(LED2_PORT,LED2_BIT)
#define LED2_TOGGLE   LED2_PTOR(LED2_PORT,LED2_BIT)
#endif

#ifdef  LED3_PORT
#define LED3
#define LED3_PCR_OUTPUT(LED3_PORT,LED3_BIT)  PCR(LED3_PORT,LED3_BIT)
#define LED3_DDR_OUTPUT(LED3_PORT,LED3_BIT)  PDDR(LED3_PORT) |= (1<<LED3_BIT)
#define LED3_PSOR(LED3_PORT,LED3_BIT)        PSOR(LED3_PORT) |= (1<<LED3_BIT)
#define LED3_PCOR(LED3_PORT,LED3_BIT)        PCOR(LED3_PORT) |= (1<<LED3_BIT)
#define LED3_PTOR(LED3_PORT,LED3_BIT)        PTOR(LED3_PORT) |= (1<<LED3_BIT)

#define LED3_PCR        LED3_PCR_OUTPUT (LED3_PORT,LED3_BIT)
#define LED3_OUTPUT_EN   LED3_DDR_OUTPUT (LED3_PORT,LED3_BIT)

#define LED3_CLR       LED3_PCOR(LED3_PORT,LED3_BIT) //Led turn with positive
#define LED3_SET       LED3_PSOR(LED3_PORT,LED3_BIT)

#define LED3_OFF       LED3_PSOR(LED3_PORT,LED3_BIT) //Led turn with positive
#define LED3_ON        LED3_PCOR(LED3_PORT,LED3_BIT)
#define LED3_TOGGLE   LED3_PTOR(LED3_PORT,LED3_BIT)
#endif


#ifdef  LED4_PORT                               
#define LED4
#define LED4_PCR_OUTPUT(LED4_PORT,LED4_BIT)  PCR(LED4_PORT,LED4_BIT)
#define LED4_DDR_OUTPUT(LED4_PORT,LED4_BIT)  PDDR(LED4_PORT) |= (1<<LED4_BIT)
#define LED4_PSOR(LED4_PORT,LED4_BIT)        PSOR(LED4_PORT) |= (1<<LED4_BIT)
#define LED4_PCOR(LED4_PORT,LED4_BIT)        PCOR(LED4_PORT) |= (1<<LED4_BIT)
#define LED4_PTOR(LED4_PORT,LED4_BIT)        PTOR(LED4_PORT) |= (1<<LED4_BIT)
#define LED4_PDOR(LED4_PORT,LED4_BIT)        PDOR(LED4_PORT) & (1<<LED4_BIT)


#define LED4_PCR        LED4_PCR_OUTPUT (LED4_PORT,LED4_BIT)
#define LED4_OUTPUT_EN   LED4_DDR_OUTPUT (LED4_PORT,LED4_BIT)

#define LED4_CLR    LED4_PCOR(LED4_PORT,LED4_BIT)    //Led turn with positive
#define LED4_SET    LED4_PSOR(LED4_PORT,LED4_BIT)

#define LED4_OFF      LED4_PSOR(LED4_PORT,LED4_BIT)    //Led turn with positive
#define LED4_ON       LED4_PCOR(LED4_PORT,LED4_BIT)
#define LED4_TOGGLE   LED4_PTOR(LED4_PORT,LED4_BIT)
#endif



#ifdef  SW1_PORT
#define SW1
#define SW1_PCR_INPUT(SW1_PORT,SW1_BIT)  PCR(SW1_PORT,SW1_BIT)
#define SW1_DDR_INPUT(SW1_PORT,SW1_BIT)  PDDR(SW1_PORT) &= ~(1<<SW1_BIT)
#define SW1_DIR_INPUT(SW1_PORT,SW1_BIT)  (PDIR(SW1_PORT) & (1<<SW1_BIT))

#define SW1_PCR         SW1_PCR_INPUT(SW1_PORT,SW1_BIT)
#define SW1_INPUT_EN    SW1_DDR_INPUT(SW1_PORT,SW1_BIT)
#define SW1_VAL         SW1_DIR_INPUT(SW1_PORT,SW1_BIT)
#define SW1_ON          !SW1_VAL
#endif


#ifdef  SW2_PORT
#define SW2
#define SW2_PCR_INPUT(SW2_PORT,SW2_BIT)  PCR(SW2_PORT,SW2_BIT)
#define SW2_DDR_INPUT(SW2_PORT,SW2_BIT)  PDDR(SW2_PORT) &= ~(1<<SW2_BIT)
#define SW2_DIR_INPUT(SW2_PORT,SW2_BIT)  (PDIR(SW2_PORT) & (1<<SW2_BIT))

#define SW2_PCR         SW2_PCR_INPUT(SW2_PORT,SW2_BIT)
#define SW2_INPUT_EN    SW2_DDR_INPUT(SW2_PORT,SW2_BIT)
#define SW2_VAL         SW2_DIR_INPUT(SW2_PORT,SW2_BIT)
#define SW2_ON          !SW2_VAL
#endif


#ifdef  SCS_PORT
#define SCS
#define SCS_PCR_OUTPUT(SCS_PORT,SCS_BIT)  PCR(SCS_PORT,SCS_BIT)
#define SCS_DDR_OUTPUT(SCS_PORT,SCS_BIT)  PDDR(SCS_PORT) |= (1<<SCS_BIT)
#define SCS_PSOR(SCS_PORT,SCS_BIT)        PSOR(SCS_PORT) |= (1<<SCS_BIT)
#define SCS_PCOR(SCS_PORT,SCS_BIT)        PCOR(SCS_PORT) |= (1<<SCS_BIT)
#define SCS_PTOR(SCS_PORT,SCS_BIT)        PTOR(SCS_PORT) |= (1<<SCS_BIT)

#define SCS_PCR        SCS_PCR_OUTPUT(SCS_PORT,SCS_BIT)
#define SCS_OUTPUT_EN  SCS_DDR_OUTPUT(SCS_PORT,SCS_BIT)

#define SCS_SET         SCS_PSOR(SCS_PORT,SCS_BIT)
#define SCS_CLR         SCS_PCOR(SCS_PORT,SCS_BIT)

#define SCS_ON         SCS_PCOR(SCS_PORT,SCS_BIT)
#define SCS_OFF        SCS_PSOR(SCS_PORT,SCS_BIT)
#define SCS_TOGGLE     SCS_PTOR(SCS_PORT,SCS_BIT)
#endif

#ifdef  RST_PORT
#define RST
#define RST_PCR_OUTPUT(RST_PORT,RST_BIT)  PCR(RST_PORT,RST_BIT)
#define RST_DDR_OUTPUT(RST_PORT,RST_BIT)  PDDR(RST_PORT) |= (1<<RST_BIT)
#define RST_PSOR(RST_PORT,RST_BIT)        PSOR(RST_PORT) |= (1<<RST_BIT)
#define RST_PCOR(RST_PORT,RST_BIT)        PCOR(RST_PORT) |= (1<<RST_BIT)
#define RST_PTOR(RST_PORT,RST_BIT)        PTOR(RST_PORT) |= (1<<RST_BIT)

#define RST_PCR        RST_PCR_OUTPUT(RST_PORT,RST_BIT)
#define RST_OUTPUT_EN  RST_DDR_OUTPUT(RST_PORT,RST_BIT)

#define RST_SET         RST_PSOR(RST_PORT,RST_BIT)
#define RST_CLR         RST_PCOR(RST_PORT,RST_BIT)

#define RST_ON         RST_PCOR(RST_PORT,RST_BIT)
#define RST_OFF        RST_PSOR(RST_PORT,RST_BIT)
#define RST_TOGGLE     RST_PTOR(RST_PORT,RST_BIT)
#endif






void  gpio_init(void);
char input_rise(char input,char *mem);
char input_fall(char input,char *mem);
