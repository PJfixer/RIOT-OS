#include <stdio.h>
#include "shell.h"
#include <string.h>
#include <board.h>
#include <periph/gpio.h>
#include "periph/uart.h"
#include <xtimer.h>

char *inputString;
uint8_t stringComplete = 0 ;
int target_handler(int argc,char**argv){
	(void)argv;
	(void)argc;
	printf("%s \n",RIOT_BOARD);
	printf("%s \n",RIOT_CPU);
return 0;
}

int custom_handler(int argc,char**argv){

if(argc > 1)
{
	printf(" you type:  %s",argv[1]);
}
return 0;
}
	
int led0_handler(int argc,char**argv){

if(argc > 1)
{
        if(!strcmp(argv[1],"1"))
	{	gpio_set(LED0_PIN);
		printf("LED ON\n");
		return 0; 

	}
	else if(!strcmp(argv[1],"0"))
        {
		gpio_clear(LED0_PIN);
        	printf("LED0 OFF\n");
		return 0;
        }
	else if(!strcmp(argv[1],"x"))
        {
		gpio_toggle(LED0_PIN);
        	printf("LED0 Toggled");
		return 0;
        }

}
return -1;
}

void *blink_thread(void *arg)
{
	(void)arg;
xtimer_ticks32_t   last_wakeup = xtimer_now() ;
  printf("UN THREAD \n !!");
while(1)
{
	
	//xtimer_sleep(1);
	//uint8_t test = 'P' ;
	
	xtimer_periodic_wakeup(&last_wakeup,1000000);
	
	if(stringComplete == 1 )
	{
		UART1_SendStr((const char *)"complete frame received \n");
		stringComplete = 0 ;
	}
	
}

	return NULL;
}



void UART1_SendStr( const char * s)
{
    while(*s)
            
            uart_write(UART_DEV(1),(const uint8_t *)*s++,1);
}


void receive_uart(void *arg,uint8_t data)
{
	(void)arg;
	gpio_toggle(LED0_PIN);
	char inChar = (char)data;
   
    inputString += inChar;
    if (inChar == 'K') {
      stringComplete = 1;
	}
}


  char blink_thread_stack[THREAD_STACKSIZE_MAIN];

 

int main(void)
{
	gpio_init(LED0_PIN,GPIO_OUT);
	uart_poweron(UART_DEV(1));
	if(uart_init (UART_DEV(1),9600,(uart_rx_cb_t)receive_uart,NULL) !=0)
	{
			printf("Uart init failed ( as always ) \r\n");
}
UART1_SendStr((const char *)"hello from uart1\n");
	 //xtimer_ticks32_t *     last_wakeup;
	//char blink_thread_stack[THREAD_STACKSIZE_MAIN];
	thread_create(blink_thread_stack, sizeof(blink_thread_stack),THREAD_PRIORITY_MAIN+5, THREAD_CREATE_STACKTEST,blink_thread, NULL, "blink_thread");
	printf("First IOT hello wolrd \r\n");
	//gpio_init(LED0_PIN,GPIO_OUT);
	static const shell_command_t shell_commands[] = {{"target","display board", target_handler },{"custom","custom commands", custom_handler },{"LED0","set led0 state", led0_handler },{ NULL, NULL, NULL }};
	char line_buf[SHELL_DEFAULT_BUFSIZE];
	shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
	return 0;	
	
}




