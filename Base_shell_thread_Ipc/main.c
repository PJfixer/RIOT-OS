#include <stdio.h>
#include "shell.h"
#include <string.h>
#include "board.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include <msg.h>

static kernel_pid_t wait_pid;


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
#define PA5 GPIO_PIN(PORT_A, 5)
#define PC13 GPIO_PIN(PORT_C, 13)

void myCallback(void *arg)
{
	(void)arg;
	msg_t ISRmsg;
	ISRmsg.type = 2;
	ISRmsg.content.value = 1;
	msg_send(&ISRmsg,wait_pid);
}



void *waiting_thread(void *arg)
{
	(void)arg;
	wait_pid =  thread_getpid();
	msg_t m;
 	printf("COPY THREAD :D !! \n ");
	gpio_init(PA5,GPIO_OUT);
	//gpio_init(PC13,GPIO_IN);
	//gpio_init_int (PC13,GPIO_IN,GPIO_FALLING,myCallback,NULL);
	while(1)
	{
		msg_receive(&m);
		gpio_toggle(PA5);

	}

		return NULL;
}

char waiting_thread_stack[THREAD_STACKSIZE_MAIN];
int main(void)
{
	 gpio_init_int (PC13,GPIO_IN,GPIO_FALLING,myCallback,NULL);
	//xtimer_ticks32_t *     last_wakeup;
	//char blink_thread_stack[THREAD_STACKSIZE_MAIN];
	thread_create(waiting_thread_stack, sizeof(waiting_thread_stack),THREAD_PRIORITY_MAIN+5, THREAD_CREATE_STACKTEST,waiting_thread, NULL, "waiting_thread");
	printf("First IOT hello wolrd \r\n");
	//gpio_init(LED0_PIN,GPIO_OUT);
	static const shell_command_t shell_commands[] = {{"target","display board", target_handler },{"custom","custom commands", custom_handler },{"LED0","set led0 state", led0_handler },{ NULL, NULL, NULL }};
	char line_buf[SHELL_DEFAULT_BUFSIZE];
	shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
	return 0;

}




