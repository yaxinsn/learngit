/******************************************
 * FILE:           libpim.h
 * author:         yaxinsn
 * version:        1
 *
 * desception:   
 *              
 *
 * key function:   
 * lib example:    .
*******************************************/                                                                         
#ifndef _LIBPIM_H
#define _LIBPIM_H 

#include <stdio.h>
#include <pthread.h>
#include <sys/queue.h>
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>




typedef struct im{
	TAILQ_ENTRY(im);
	int im_id;
	int im_src;
	int im_dst;
	char msg[0];
}im_t;

typedef struct im_ctrl{
	im_t* im;
	pthread_t pid;
	int wake[2];
	pthread_mutex_t lock;
}im_ctrl_t;

im_ctrl_t* im_create(void);


void im_destroy(im_ctrl_t* imc);


#endif   //endif of 
