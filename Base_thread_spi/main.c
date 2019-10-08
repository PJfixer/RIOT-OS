#include <stdio.h>
#include "shell.h"
#include <string.h>
#include "board.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "msg.h"
#include "periph/spi.h"
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
	/*msg_t ISRmsg;
	ISRmsg.type = 2;
	ISRmsg.content.value = 1;
	msg_send(&ISRmsg,wait_pid);*/
}


 


int max31855_init(spi_t bus,spi_cs_t cs)
{
	spi_init_cs (bus,cs);
	return 0;
}
int max31855_readtemp(spi_t bus,spi_cs_t cs)
{

 uint8_t raw[4];

spi_acquire (bus,cs,SPI_MODE_0,SPI_CLK_100KHZ );
raw[0] = spi_transfer_byte(bus,cs,true,0x00);
raw[1] = spi_transfer_byte(bus,cs,true,0x00);
raw[2] = spi_transfer_byte(bus,cs,true,0x00);
raw[3] = spi_transfer_byte(bus,cs,false,0x00);
//printf("bip ! \n");
//printf("raw[0] %d \n",raw[0]);
//printf("raw[1] %d \n",raw[1]);
//printf("raw[2] %d \n",raw[2]);
//printf("raw[3] %d \n",raw[3]); 
uint16_t K = ((raw[0]<<8)|raw[1]);
K = (K>>2);

//printf("K : %d",K);
printf("Temp : %f °C \n",((float)K)*0.25);
 
spi_release(bus);
return 0;

}


void *waiting_thread(void *arg)
{
	(void)arg;
	wait_pid =  thread_getpid();
	//msg_t m;

	while(1)
	{
		//msg_receive(&m);
		max31855_readtemp(SPI_DEV(0),GPIO_PIN(PORT_A,4));
		xtimer_sleep(1);
	}

		return NULL;
}

char waiting_thread_stack[THREAD_STACKSIZE_MAIN];
int main(void)
{
	
	spi_init_cs(SPI_DEV(0),GPIO_PIN(PORT_A,4));
	gpio_init_int (PC13,GPIO_IN,GPIO_FALLING,myCallback,NULL);
	gpio_init(GPIO_PIN(PORT_A,4),GPIO_OUT);
	thread_create(waiting_thread_stack, sizeof(waiting_thread_stack),THREAD_PRIORITY_MAIN+5, THREAD_CREATE_STACKTEST,waiting_thread, NULL, "waiting_thread");
	printf("First IOT hello wolrd \r\n");
	static const shell_command_t shell_commands[] = {{"target","display board", target_handler },{"custom","custom commands", custom_handler },{"LED0","set led0 state", led0_handler },{ NULL, NULL, NULL }};
	char line_buf[SHELL_DEFAULT_BUFSIZE];
	shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);
	return 0;

}




