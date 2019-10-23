#include <stdio.h>
#include "shell.h"
#include <string.h>
#include <board.h>
#include <periph/gpio.h>
#include "periph/hwrng.h"
#include <xtimer.h>
#include "lcd_grove.h"
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
xtimer_ticks32_t   last_wakeup = xtimer_now();
  printf("UN THREAD \n !!");
while(1)
{
	gpio_toggle(LED0_PIN);
	//xtimer_sleep(1);
	
	xtimer_periodic_wakeup(&last_wakeup,1000000);
		
}

	return NULL;
}


void *rand_thread(void *arg)
{
	(void)arg;
xtimer_ticks32_t   last_wakeup = xtimer_now();
  printf("RAND THREAD \n !!");
while(1)
{
	 uint8_t rand_red;
	 uint8_t rand_green;
	 uint8_t rand_blue;
	 hwrng_read(&rand_red,1);
	 hwrng_read(&rand_green,1);
	 hwrng_read(&rand_blue,1);	 
	setRGB(I2C_DEV(0),rand_red,rand_green,rand_blue);
	xtimer_periodic_wakeup(&last_wakeup,1000000);
	
}

	return NULL;
}


  char blink_thread_stack[THREAD_STACKSIZE_MAIN];
  char rand_thread_stack[THREAD_STACKSIZE_MAIN];


int main(void)
{
	hwrng_init();
	
	gpio_init(LED0_PIN,GPIO_OUT);
	 //xtimer_ticks32_t *     last_wakeup;
	//char blink_thread_stack[THREAD_STACKSIZE_MAIN];
	thread_create(blink_thread_stack, sizeof(blink_thread_stack),THREAD_PRIORITY_MAIN+5, THREAD_CREATE_STACKTEST,blink_thread, NULL, "blink_thread");
	thread_create(rand_thread_stack, sizeof(rand_thread_stack),THREAD_PRIORITY_MAIN+4, THREAD_CREATE_STACKTEST,rand_thread, NULL, "rand_thread");
	printf("First IOT hello wolrd \r\n");
	print_test();
	if(lcd_init(I2C_DEV(0)) != 0)
	{
		printf("init failed \r\n");
	}
	if(lcd_displayOn(I2C_DEV(0)) != 0)
	{
		printf("display on  failed \r\n");
	}
	if(lcd_clear(I2C_DEV(0)) != 0)
	{
		printf("clear failed \r\n");
	}
	if(lcd_sendByte(I2C_DEV(0),'A') != 0)
	{
		printf("write A failed \r\n");
	}
	lcd_SendStr(I2C_DEV(0)," YOLO");
	print_test();
	
	//gpio_init(LED0_PIN,GPIO_OUT);
	static const shell_command_t shell_commands[] = {{"target","display board", target_handler },{"custom","custom commands", custom_handler },{"LED0","set led0 state", led0_handler },{ NULL, NULL, NULL }};
	char line_buf[SHELL_DEFAULT_BUFSIZE];
	shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
	return 0;	
	
}




