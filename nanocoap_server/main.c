/*
 * Copyright (C) 2016 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       CoAP example server application (using nanocoap)
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @}
 */

#include <stdio.h>

#include "net/nanocoap_sock.h"
#include "xtimer.h"


#include "inttypes.h"
#include "board.h"

#include "lpsxxx.h"
#include "lpsxxx_params.h"


#define SAMPLING_PERIOD     2

#define COAP_INBUF_SIZE (256U)

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

/* import "ifconfig" shell command, used for printing addresses */
extern int _gnrc_netif_config(int argc, char **argv);

 uint16_t pres;
 int16_t temp;
 int temp_abs; 
 

void *temp_thread(void *arg)
{
	(void)arg;
	 lpsxxx_t dev;
	 printf("Test application for %s pressure sensor\n\n", LPSXXX_SAUL_NAME);
    printf("Initializing %s sensor\n", LPSXXX_SAUL_NAME);
    if (lpsxxx_init(&dev, &lpsxxx_params[0]) != LPSXXX_OK) {
        puts("Initialization failed");
        //return 1;
    }

	xtimer_ticks32_t   last_wakeup = xtimer_now() ;
	printf("UN THREAD de temperature  \n !!");
  
while(1)
{

	lpsxxx_enable(&dev);
        xtimer_sleep(1); /* wait a bit for the measurements to complete */

        lpsxxx_read_temp(&dev, &temp);
        lpsxxx_read_pres(&dev, &pres);
        lpsxxx_disable(&dev);

        temp_abs = temp / 100;
        temp -= temp_abs * 100;

        printf("Pressure value: %ihPa - Temperature: %2i.%02i°C\n",
               pres, temp_abs, temp);
	
	xtimer_periodic_wakeup(&last_wakeup,2000000);
		
}

}

char temp_thread_stack[THREAD_STACKSIZE_MAIN];

int main(void)
{
	
	thread_create(temp_thread_stack, sizeof(temp_thread_stack),THREAD_PRIORITY_MAIN+5, THREAD_CREATE_STACKTEST,temp_thread, NULL, "temp_thread");
    puts("RIOT nanocoap example application");

    /* nanocoap_server uses gnrc sock which uses gnrc which needs a msg queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    puts("Waiting for address autoconfiguration...");
    xtimer_sleep(3);

    /* print network addresses */
    puts("Configured network interfaces:");
    _gnrc_netif_config(0, NULL);

    /* initialize nanocoap server instance */
    uint8_t buf[COAP_INBUF_SIZE];
    sock_udp_ep_t local = { .port=COAP_PORT, .family=AF_INET6 };
    nanocoap_server(&local, buf, sizeof(buf));
    
    

    return 0;
}
    
 

