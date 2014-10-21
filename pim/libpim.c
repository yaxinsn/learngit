/******************************************
 * FILE:           libpim.c
 * author:         yaxinsn
 * version:        1
 *
 * desception:   
 *              
 *
 * key function:   
 * lib example:    .
*******************************************/                                                                         


#include <stdio.h>
#include <pthread.h>
#include <sys/queue.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "libpim.h"
/*  pthread body */
static void _im_handler(void)
{
	while(1)
	{
		sleep(2);
	}
}
im_ctrl_t* im_create(void)
{
	im_ctrl_t* imc = NULL;
	int ret;	
	imc = malloc(sizeof(im_ctrl_t));
	if(!imc)
		return NULL;
	 pthread_mutex_init(&(imc->lock),NULL);  
    	 ret=pthread_create(&imc->pid,NULL,(void  *) _im_handler,NULL);  
   	 if(ret!=0)  
   	 {  
       		 printf("Create pthread error!\n");  
   		 goto err;
    	}

	if(socketpair(AF_UNIX,SOCK_DGRAM,0,imc->wake))
	{
		printf("socetkpair error %d\n",errno);
		goto err;
	}

	return imc;
err:
	pthread_mutex_destroy(&(imc->lock));
	free(imc);
	return NULL;



}

static void _message_clearup(im_ctrl_t* imc)
{
	//lock
	// todo handler
	//unlock
	return;
} 
void im_destroy(im_ctrl_t* imc)
{
        close(imc->wake[0]);	
        close(imc->wake[1]);
	_message_clearup(imc);
	pthread_mutex_destroy(&(imc->lock));
	free(imc);
	return;
}
