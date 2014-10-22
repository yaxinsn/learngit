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
#include <sys/time.h>
#include <unistd.h>
#include <sys/select.h>
#include "libpim.h"


/*  pthread body */
static void _im_handler(void* arg)
{
	fd_set fds;
	char wake_buf[20];
	im_ctrl_t* imc = (im_ctrl_t*)arg;
	//struct timeval timeout={3,0};
	
	while(1)
	{
		if(imc->stop == 1)
		{
			printf("I will stoped!\n");
			pthread_exit(0);
		}
		FD_ZERO(&fds);
		FD_SET(imc->wake[0],&fds);
		switch(select(imc->wake[0]+1,&fds,NULL,NULL,NULL))
		{
			case -1:
				printf("selcet:%d\n",errno);
				exit(1);
				break;
			case 0:
				break;
			default:
				read(imc->wake[0],wake_buf,20);
				printf("I(%u) wake up by %s \n",imc->pid,wake_buf);
			//wake this pthread and read imc->im;
			break;	
		}
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
    	if(socketpair(AF_UNIX,SOCK_DGRAM,0,imc->wake))
	{
		printf("socetkpair error %d\n",errno);
		goto err;
	}

	 ret=pthread_create(&imc->pid,NULL,(void  *) _im_handler,imc);  
   	 if(ret!=0)  
   	 {
       		 printf("Create pthread error!\n");  
   		 goto err2;
    	}

	return imc;
err2:
	close(imc->wake[0]);
	close(imc->wake[1]);
err:
	pthread_mutex_destroy(&(imc->lock));
	free(imc);
	return NULL;



}
static void _im_wake_up(im_ctrl_t* imc)
{
	write(imc->wake[1],"abc",3);
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
	_im_wake_up(imc);
	imc->stop = 1;
	pthread_join(imc->pid,NULL);
        close(imc->wake[0]);	
        close(imc->wake[1]);
	_message_clearup(imc);
	
	pthread_mutex_destroy(&(imc->lock));
	free(imc);
	return;
}
int im_push_msg(im_ctrl_t* imc,unsigned char* msg,int len)
{
	//malloc and copy and insert to tailq
	_im_wake_up(imc);
	return 1;
}
