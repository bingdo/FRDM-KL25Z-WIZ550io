//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include "diag/Trace.h"

//#include "Timer.h"
//#include "BlinkLed.h"

#include "freedom_gpio.h"
#include "freedom_spi.h"
#include "common.h"
#include "wizchip_conf.h"

// ----------------------------------------------------------------------------
//
// KL25 led blink sample (trace via STDOUT).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// To demonstrate POSIX retargetting, reroute the STDOUT and STDERR to the
// trace device and display messages on both of them.
//
// Then demonstrates how to blink a led with 1Hz, using a
// continuous loop and SysTick delays.
//
// On DEBUG, the uptime in seconds is also displayed on the trace device.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the STDOUT output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 2 / 3)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/////////////////////////////////////////
// SOCKET NUMBER DEFINION for Examples //
/////////////////////////////////////////
#define SOCK_TCPS        0
#define SOCK_UDPS        1
#define SOCK_DNS		 6
#define SOCK_HTTP		 7

////////////////////////////////////////////////
// Shared Buffer Definition for LOOPBACK TEST //
////////////////////////////////////////////////

uint8_t gDATABUF[DATA_BUF_SIZE];

///////////////////////////////////
// Default Network Configuration //
///////////////////////////////////
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},
                            .ip = {192, 168, 1, 90},
                            .sn = {255,255,255,0},
                            .gw = {192, 168, 1, 1},
                            .dns = {8,8,8,8},
                            .dhcp = NETINFO_STATIC };

//////////////////////////////////////////////////////////////////////////////////////////////
// Call back function for W5500 SPI - Theses used as parameter of reg_wizchip_xxx_cbfunc()  //
// Should be implemented by WIZCHIP users because host is dependent                         //
//////////////////////////////////////////////////////////////////////////////////////////////
void  wizchip_select(void);
void  wizchip_deselect(void);
void  wizchip_write(uint8_t wb);
uint8_t wizchip_read();

void network_init(void);

int
main(int argc, char* argv[])
{
  //uint32_t seconds = 0;
  uint8_t tmp = 0;
  //uint8_t ret = 0;
  //uint8_t sn = 0;

  uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};

  // By customising __initialize_args() it is possible to pass arguments,
  // for example when running tests with semihosting you can pass various
  // options to the test.
  // trace_dump_args(argc, argv);

  // Send a greeting to the trace device (skipped on Release).
  trace_puts("Hello ARM World!");

  // The standard output and the standard error should be forwarded to
  // the trace device. For this to work, a redirection in _write.c is
  // required.
  puts("Standard output message.");
  fprintf(stderr, "Standard error message.\n");

  // At this stage the system clock should have already been configured
  // at high speed.
  trace_printf("System clock: %uHz\n", SystemCoreClock);

  //timer_start();

  //blink_led_init();
  
  //uint32_t seconds = 0;

  gpio_init();
  spi_init();

  wizchip_deselect();
  reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
  /* SPI read & write callback function */
  reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);

  /* WIZCHIP SOCKET Buffer initialize */
  if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
  {
	  trace_printf("WIZCHIP Initialized fail.\r\n");
     while(1);
  }
  /* PHY link status check */
  do
  {
     if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
    	 trace_printf("Unknown PHY Link stauts.\r\n");
  }while(tmp == PHY_LINK_OFF);
  
  /* Network Initialization */
  network_init();

  // Infinite loop
  while (1)
  {
      //blink_led_on();
	  //LED1_ON;
      //timer_sleep(BLINK_ON_TICKS);

      //blink_led_off();
	  //LED1_OFF;
      //timer_sleep(BLINK_OFF_TICKS);

      //++seconds;

      // Count seconds on the trace device.
      //trace_printf("Second %u\n", seconds);
  }
  // Infinite loop, never return.
}

void  wizchip_select(void)
{
	SCS_ON;
}

void  wizchip_deselect(void)
{
	SCS_OFF;
}

void  wizchip_write(uint8_t wb)
{
	uint8_t  byte;

	while(!(SPI_S_SPTEF_MASK & SPI0_S))
	{
		asm("nop");  //While buffer is not empty do nothing
	}
	SPI0_D = wb;    //Write char to SPI

	SPI0_D = 0x00;
	while(!(SPI_S_SPRF_MASK & SPI0_S))
	{
		asm("nop");  //While buffer is not empty do nothing
	} // Wait for receive flag to set
	byte = SPI0_D;

}

uint8_t wizchip_read()
{
	uint8_t  byte;

	while(!(SPI_S_SPTEF_MASK & SPI0_S))
	{
		asm("nop");  //While buffer is not empty do nothing
	}
	//SPI0_D = wb;    //Write char to SPI

	SPI0_D = 0x00;
	while(!(SPI_S_SPRF_MASK & SPI0_S))
	{
		asm("nop");  //While buffer is not empty do nothing
	} // Wait for receive flag to set
	byte = SPI0_D;

	return byte;  //Read SPI data from slave
}

/////////////////////////////////////////////////////////////
// Initialize the network information to be used in WIZCHIP //
/////////////////////////////////////////////////////////////
void network_init(void)
{
   uint8_t tmpstr[6];


//	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);

#if 0
    gWIZNETINFO.ip[0] = 222;
    gWIZNETINFO.ip[1] = 98;
    gWIZNETINFO.ip[2] = 173;
    gWIZNETINFO.ip[3] = 216;
    gWIZNETINFO.gw[0] = 222;
    gWIZNETINFO.gw[1] = 98;
    gWIZNETINFO.gw[2] = 173;
    gWIZNETINFO.gw[3] = 254;
    gWIZNETINFO.sn[0] = 255;
    gWIZNETINFO.sn[1] = 255;
    gWIZNETINFO.sn[2] = 255;
    gWIZNETINFO.sn[3] = 192;
    gWIZNETINFO.dns[0] = 8;
    gWIZNETINFO.dns[1] = 8;
    gWIZNETINFO.dns[2] = 8;
    gWIZNETINFO.dns[3] = 8;
    gWIZNETINFO.dhcp = NETINFO_STATIC;
#endif

    //ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
    ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	trace_printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
	trace_printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
														  gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	trace_printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	trace_printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	trace_printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	trace_printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
	trace_printf("======================\r\n");
}
/////////////////////////////////////////////////////////////

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
