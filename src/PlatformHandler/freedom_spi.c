/*
 * File:		freedom_spi.c
 * Purpose:		Main process
 *
 */

#include "common.h"
#include "freedom_gpio.h"
#include "Timer.h"

//void spi_init( void );
void spi1_dma_master_spi0_slave(void);
void DMA0_IRQHandler( void );
void DMA1_IRQHandler( void );
void DMA2_IRQHandler( void );
void DMA3_IRQHandler( void );

uint8_t m_DMA0_IntFlag;
uint8_t m_DMA1_IntFlag;
uint8_t m_DMA2_IntFlag;
uint8_t m_DMA3_IntFlag;

uint8_t rdata8[10],m_rdata8[10],s_rdata8[10];	 //Cash: for sram is so small
uint8_t tdata8[10],m_tdata8[10],s_tdata8[10];	 //Cash: for sram is so small
int32_t dma_int_cnt;
volatile uint8_t rd,rd1,r_data[8];
uint32_t k,count;
uint32_t global_pass_count;		//Pass flag
uint32_t global_fail_count;		//Fail flag
volatile int32_t extend_cnt;
/********************************************************************/
#if 0
int main (void)
{
    char ch;
        
  	printf("\n\rRunning the twr_spi_demo project.\n\r");
	
	printf("This demo shows the SPI communication with DMA transfer.\n\r\n\r");
	printf("It is targeted for the Freescale KL25 tower card and accompanying \n\r");
	printf("Elevator cards.  In addition, you will need access to the B side expansion port \n\r");
	printf("as all pin numbers are with respect to the B side expansion port.\n\r\n\r");
	printf("SPI1 will be configured as a master and SPI0 will be configured as slave!\n\r");
	printf("Please make the following connections and then input a character to continue:\n\r\n\r");
	printf("NOTE: All pin numbers are with respect to the B side expansion port of the primary Elevator card.\n\r");
	printf("Connect PTE2 (pin 7) to PTD1 (pin 48) -- SPI_CLK \n\r");
	printf("Connect PTE4 (pin 9) to PTD0 (pin 46) -- SPI_CS \n\r");
	printf("Connect PTE1 (pin 10) to PTD2 (pin 45) -- SPI_MOSI \n\r");
	printf("Connect PTE0 (pin 11) to PTD3 (pin 44) -- SPI_MISO \n\r\n\r");
	ch = in_char();
	
	while(1)
	{
		spi1_dma_master_spi0_slave();
		printf("\n\rPlease input char for next test!\n\r\n\r");
		ch = in_char();
		ch = ch + 1;  // Dummy usage to suppress warning.  
	} 
}
#endif

void spi_init( void )
{
#if 0
	/* SPI Initial functions module---Open clock gating and Pin mux for spi, Close COP(watchdog) with clock gating or ---  */
	/**/
	//SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK|SIM_SCGC4_SPI1_MASK; 		   //Enable SPI0 Clock gate
	SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK; 		   //Enable SPI0 Clock gate
	SIM_COPC &= (~SIM_COPC_COPT_MASK);	//Disable COP Watchdog
	//SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTD_MASK|SIM_SCGC5_PORTE_MASK|SIM_SCGC5_PORTC_MASK;
   	
	// disable SPI
	//SPI1_C1 &= ~SPI_C1_SPE_MASK;
	SPI0_C1 &= ~SPI_C1_SPE_MASK;
	
    PORTD_PCR0 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR0 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;			  //Use PTD0 as SPI0_SS_b
    PORTD_PCR3 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR3 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;			  //Use PTD3 as SPI0_MISO 
    PORTD_PCR2 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR2 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;			  //Use PTD2 as SPI0_MOSI
    PORTD_PCR1 &= ~PORT_PCR_MUX_MASK;
	PORTD_PCR1 = PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;			    //Use PTD1 as SPI0_SCK
        
    //PORTE_PCR4 &= ~PORT_PCR_MUX_MASK;                   //spi1_cs
	//PORTE_PCR4 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;			  //Use PTE4 as SPI1_SS_b
    //PORTE_PCR1 &= ~PORT_PCR_MUX_MASK;                        //sin
	//PORTE_PCR1 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;			  //Use PTE1 as SPI1_MISO
    //PORTE_PCR3 &= ~PORT_PCR_MUX_MASK;                        //dout
	//PORTE_PCR3 |= PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;			  //Use PTE3 as SPI1_MOSI
    //PORTE_PCR2 &= ~PORT_PCR_MUX_MASK;                   //spi1_sck
	//PORTE_PCR2 = PORT_PCR_MUX(2)|PORT_PCR_DSE_MASK;			  //Use PTE2 as SPI1_SCK
	
	
	//SPI0_C1 |= SPI_C1_MSTR_MASK;	  //-----master----bus clock is 12.5Mhz--0.08us--
	//SPI0_BR = 0x02;
    SPI0_C1 &= (~SPI_C1_MSTR_MASK);
	//SPI0_BR = 0x43;  //SPPR = 4, SPR = 3, bps div = (SPPR+1)*2^(SPR+1) = 80,----Tspi--6.4us
	//SPI0_BR = 0x40; //bps div = 10,---------0.8us
	//SPI0_BR = 0x30; //bps div = 8,----------0.64us
	//SPI0_BR = 0x10; //bps div = 4,----------0.32us
	//SPI0_BR = 0x00; //bps div = 2,----------0.16us----6.125Mhz
	//SPI0_BR = 0x54; //bps div = 192,----------15.36us
	//SPI0_BR = 0x77; //bps div = 2048,----------163.84us
    // SPI0_BR = 0x00;
	SPI0_C1 |= SPI_C1_SSOE_MASK;      //|SPI0_C1_CPOL_MASK|SPI0_C1_LSBFE_MASK;
	SPI0_C2 |= SPI_C2_MODFEN_MASK;

	SPI0_C1 |= SPI_C1_CPHA_MASK;
	SPI0_C1 &= (~SPI_C1_CPHA_MASK);
	SPI0_C1 |= SPI_C1_CPOL_MASK;
	SPI0_C1 &= (~SPI_C1_CPOL_MASK);
	//SPI0_C1 |= SPI_C1_LSBFE_MASK;
	SPI0_C1 &= (~SPI_C1_LSBFE_MASK);

	//SPI0_C1 &= (~SPI0_C1_SPIE_MASK);     //Disable RX interrrupt
  	//SPI0_C1 |= SPI0_C1_SPIE_MASK;	       //enable RX interrrupt 		  
	//SPI0_C1 &= (~SPI0_C1_SPTIE_MASK);	   //Disable the transmit interrupt
 	//SPI0_C1 |= SPI0_C1_SPTIE_MASK;	     //Enable the transime interrupt
                
	//SPI1_C1 |= SPI_C1_MSTR_MASK;
	//SPI1_BR = 0x02;
    // SPI1_C1 &= (~SPI_C1_MSTR_MASK);	  //---slave----bus clock is 12.5Mhz--0.08us--		
	//SPI0_BR = 0x43;  //SPPR = 4, SPR = 3, bps div = (SPPR+1)*2^(SPR+1) = 80,----Tspi--6.4us
	//SPI0_BR = 0x40; //bps div = 10,---------0.8us
	//SPI0_BR = 0x30; //bps div = 8,----------0.64us
	//SPI0_BR = 0x10; //bps div = 4,----------0.32us
	//SPI0_BR = 0x00; //bps div = 2,----------0.16us----6.125Mhz
	//SPI0_BR = 0x54; //bps div = 192,----------15.36us
	//SPI0_BR = 0x77; //bps div = 2048,----------163.84us
    // SPI1_BR = 0x01;
	//SPI1_C1 |= SPI_C1_SSOE_MASK;
	//SPI1_C2 |= SPI_C2_MODFEN_MASK;

	//SPI1_C1 |= SPI_C1_CPHA_MASK;
	//SPI1_C1 &= (~SPI_C1_CPHA_MASK);
	//SPI1_C1 |= SPI_C1_CPOL_MASK;
	//SPI1_C1 &= (~SPI_C1_CPOL_MASK);
	//SPI1_C1 |= SPI_C1_LSBFE_MASK;
	//SPI1_C1 &= (~SPI_C1_LSBFE_MASK);

                
	//SPI1_C2 |= SPI_C2_RXDMAE_MASK;
	SPI0_C2 |= SPI_C2_TXDMAE_MASK;
	SPI0_C2 |= SPI_C2_RXDMAE_MASK;
	//SPI1_C2 |= SPI_C2_TXDMAE_MASK;
		
	SPI0_C1 |= SPI_C1_SPE_MASK;
	//SPI1_C1 |= SPI_C1_SPE_MASK;
#else
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;      //Turn on clock to D module
	SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;       //Enable SPI0 clock

	PORTD_PCR0 = PORT_PCR_MUX(0x1);    //Set PTD0 to mux 2 [SPI0_PCS0]
	PORTD_PCR1 = PORT_PCR_MUX(0x2);    //Set PTD1 to mux 2 [SPI0_SCK]
	PORTD_PCR2 = PORT_PCR_MUX(0x2);    //Set PTD2 to mux 2 [SPI0_MOSI]
	PORTD_PCR3 = PORT_PCR_MUX(0x2);    //Set PTD3 to mux 2 [SPIO_MISO]

	SPI0_C1 = SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK;   //Set SPI0 to Master & SS pin to auto SS
	SPI0_C2 = SPI_C2_MODFEN_MASK;   //Master SS pin acts as slave select output
	SPI0_BR = (SPI_BR_SPPR(0x02) | SPI_BR_SPR(0x08));     //Set baud rate prescale divisor to 3 & set baud rate divisor to 64 for baud rate of 15625 hz
	//SPI0_BR = 0x00;
	SPI0_C1 |= SPI_C1_SPE_MASK;    //Enable SPI0
#endif

}


void spi1_dma_master_spi0_slave(void)
{
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK; //DMAMUX Clock Gate Control: 1, clock enable---
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;   //DMA Clock Gate control: 1, clock enable----
	 
	m_tdata8[0] = 0xF0;
	m_tdata8[1] = 0x11;
	m_tdata8[2] = 0x22;
	m_tdata8[3] = 0x33;
	m_tdata8[4] = 0x44;
	m_tdata8[5] = 0x66;
	m_tdata8[6] = 0x77;
	m_tdata8[7] = 0x88;
	for (k=0; k<8; k++)
		m_rdata8[k] = 0;
                
    s_tdata8[0] = 0x01;   
	s_tdata8[1] = 0x23;
	s_tdata8[2] = 0x45;
	s_tdata8[3] = 0x67;
	s_tdata8[4] = 0x89;
	s_tdata8[5] = 0xAB;
	s_tdata8[6] = 0xCD;
	s_tdata8[7] = 0xEF;
	for (k=0; k<8; k++)
		s_rdata8[k] = 0;

#ifdef CMSIS
	disable_irq(DMA0_IRQn);    //DMA channel 0 transfer complete and error interrupt
	disable_irq(DMA1_IRQn);    //DMA channel 1 transfer complete and error	interrupt
	disable_irq(DMA2_IRQn);    //DMA channel 2 transfer complete and error	interrupt
	disable_irq(DMA3_IRQn);    //DMA channel 3 transfer complete and error	interrupt
#else
	disable_irq(0);    //DMA channel 0 transfer complete and error interrupt
	disable_irq(1);    //DMA channel 1 transfer complete and error	interrupt
	disable_irq(2);    //DMA channel 2 transfer complete and error	interrupt
	disable_irq(3);    //DMA channel 3 transfer complete and error	interrupt
#endif
	
    SIM_CLKDIV1 = ( 0
                   | SIM_CLKDIV1_OUTDIV1(0x1)
                   | SIM_CLKDIV1_OUTDIV4(0) );
	// disable DMA channel
	DMAMUX0_CHCFG0 = 0;
	DMAMUX0_CHCFG1 = 0;
	DMAMUX0_CHCFG2 = 0;
	DMAMUX0_CHCFG3 = 0;
    //
	//SPI0 receive dma source number is 16; SPI0 transmint dma source number is 17
	//*****channel 0--->TX, channel 1----->RX************
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK & (~DMAMUX_CHCFG_TRIG_MASK);
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_SOURCE(17); //TX---DMA channel 0-SPI0 source number--ENBL=1--TRIG=0---
	 
	DMAMUX0_CHCFG1 |= DMAMUX_CHCFG_ENBL_MASK & (~DMAMUX_CHCFG_TRIG_MASK);
	DMAMUX0_CHCFG1 |= DMAMUX_CHCFG_SOURCE(16); //RX---DMA channel 1-SPI0 source number--ENBL=1--TRIG=0---
         
    //*****channel 2--->TX, channel 3----->RX************
	DMAMUX0_CHCFG2 |= DMAMUX_CHCFG_ENBL_MASK & (~DMAMUX_CHCFG_TRIG_MASK);
	DMAMUX0_CHCFG2 |= DMAMUX_CHCFG_SOURCE(19); //TX---DMA channel 2-SPI1 source number--ENBL=1--TRIG=0---
	 
	DMAMUX0_CHCFG3 |= DMAMUX_CHCFG_ENBL_MASK & (~DMAMUX_CHCFG_TRIG_MASK);
	DMAMUX0_CHCFG3 |= DMAMUX_CHCFG_SOURCE(18); //RX---DMA channel 3-SPI1 source number--ENBL=1--TRIG=0---
	//************channel request number ???????*****************
	 

	DMA_SAR0 = (uint32_t)(&(s_tdata8));
	DMA_DAR0 = (uint32_t)(&(SPI0_D));

	DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(8);    //BCR contains the number of bytes yet to be transferred for a given block
	DMA_DCR0 = DMA_DCR_ERQ_MASK|DMA_DCR_EINT_MASK|DMA_DCR_D_REQ_MASK|DMA_DCR_CS_MASK
	 		   |DMA_DCR_DSIZE(1)|DMA_DCR_SSIZE(1)|DMA_DCR_SINC_MASK;			  //|DMA_DCR_CS_MASK

	DMA_SAR1 = (uint32_t)(&(SPI0_D));
	DMA_DAR1 = (uint32_t)(&(s_rdata8));
 
	DMA_DSR_BCR1 |= DMA_DSR_BCR_BCR(8);			//BCR contains the number of bytes yet to be transferred for a given block
	DMA_DCR1 = DMA_DCR_ERQ_MASK|DMA_DCR_EINT_MASK|DMA_DCR_D_REQ_MASK|DMA_DCR_CS_MASK
	 		   |DMA_DCR_DSIZE(1)|DMA_DCR_SSIZE(1)|DMA_DCR_DINC_MASK;			  //|DMA_DCR_CS_MASK
         
    DMA_SAR2 = (uint32_t)(&(m_tdata8));			   //----tx-----
	DMA_DAR2 = (uint32_t)(&(SPI1_D));
 
	DMA_DSR_BCR2 |= DMA_DSR_BCR_BCR(8);    //BCR contains the number of bytes yet to be transferred for a given block
	DMA_DCR2 = DMA_DCR_ERQ_MASK|DMA_DCR_EINT_MASK|DMA_DCR_D_REQ_MASK|DMA_DCR_CS_MASK
	 		   |DMA_DCR_DSIZE(1)|DMA_DCR_SSIZE(1)|DMA_DCR_SINC_MASK;			  //|DMA_DCR_CS_MASK

	DMA_SAR3 = (uint32_t)(&(SPI1_D));			  //----rx-----
	DMA_DAR3 = (uint32_t)(&(m_rdata8));

	DMA_DSR_BCR3 |= DMA_DSR_BCR_BCR(8);			//BCR contains the number of bytes yet to be transferred for a given block
	DMA_DCR3 = DMA_DCR_ERQ_MASK|DMA_DCR_EINT_MASK|DMA_DCR_D_REQ_MASK|DMA_DCR_CS_MASK
	 		   |DMA_DCR_DSIZE(1)|DMA_DCR_SSIZE(1)|DMA_DCR_DINC_MASK;			  //|DMA_DCR_CS_MASK
				 
#ifdef CMSIS
	enable_irq(DMA0_IRQn);
	enable_irq(DMA1_IRQn);
	enable_irq(DMA2_IRQn);
	enable_irq(DMA3_IRQn);
#else
	enable_irq(0);
	enable_irq(1);
	enable_irq(2);
	enable_irq(3);
#endif
	
	dma_int_cnt = 0;			//this variable is used to indicates the count of transmission interrupt
	extend_cnt = 0;
	m_DMA0_IntFlag = 0;
	m_DMA1_IntFlag = 0;
	m_DMA2_IntFlag = 0;
	m_DMA3_IntFlag = 0;

	printf("Initializing SPI module!\n\r");
	spi_init();
               
	while(!m_DMA0_IntFlag);

    while(!m_DMA3_IntFlag);

             
    for(k=0; k<8 ; k++)
    {
        if(s_tdata8[k] != m_rdata8[k])
        {  
            printf("k = 0x%01x\n\r",k); 
            printf("m_tdata8 = 0x%02x\n\r",m_rdata8[k]); 
            printf("s_rdata8 = 0x%02x\n\r",s_tdata8[k]); 
     //       error_count++;
            printf("Transmit failure!\n\r");
        }
        else
        {
            printf("m_tdata8 = 0x%02x\n\r",m_rdata8[k]); 
            printf("s_rdata8 = 0x%02x\n\r",s_tdata8[k]); 
            printf("Transmit successful!\n\r");
        }
    
    }

}


void DMA0_IRQHandler( void )
{
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
	m_DMA0_IntFlag = 1;
	dma_int_cnt++;
}

void DMA1_IRQHandler( void )
{
	DMA_DSR_BCR1 |= DMA_DSR_BCR_DONE_MASK;
	m_DMA1_IntFlag = 1;
	dma_int_cnt++;
}

void DMA2_IRQHandler( void )
{
	DMA_DSR_BCR2 |= DMA_DSR_BCR_DONE_MASK;
	m_DMA2_IntFlag = 1;
	dma_int_cnt++;
}

void DMA3_IRQHandler( void )
{
	DMA_DSR_BCR3 |= DMA_DSR_BCR_DONE_MASK;
	m_DMA3_IntFlag = 1;
	dma_int_cnt++;
}

/********************************************************************/
