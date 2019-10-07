#include <stdio.h>
#include "shell.h"
#include <string.h>
#include <board.h>
#include <periph/gpio.h>
#include <xtimer.h>
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
xtimer_ticks32_t *  last_wakeup = 0;
  printf("UN THREAD \n !!");
while(1)
{
	gpio_toggle(LED0_PIN);
	//xtimer_sleep(1);
	
	xtimer_periodic_wakeup(last_wakeup,1000000);
		
}

	return NULL;
}


  char blink_thread_stack[THREAD_STACKSIZE_MAIN];


int main(void)
{
	gpio_init(LED0_PIN,GPIO_OUT);
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




