
/******************************************
FILE: 		syn-flood.c
author:		yaxinsn
version:		1

desception:   the raw of socket send IP TCP special packet.
			so this progam send tcp syn packet. so 

key function:	
			checksum
			parse_option
lib example:	getopt_long,socket.sendto.

                   _ooOoo_
                  o8888888o
                  88" . "88
                  (| -_- |)
                  O\  =  /O
               ____/`---'\____
             .'  \\|     |//  `.
            /  \\|||  :  |||//  \
           /  _||||| -:- |||||-  \
           |   | \\\  -  /// |   |
           | \_|  ''\---/''  |   |
           \  .-\__  `-`  ___/-. /
         ___`. .'  /--.--\  `. . __
      ."" '<  `.___\_<|>_/___.'  >'"".
     | | :  `- \`.;`\ _ /`;.`/ - ` : | |
     \  \ `-.   \_ __\ /__ _/   .-` /  /
======`-.____`-.___\_____/___.-`____.-'======
                   `=---='
          Buddha bless never BUG
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

******************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "parse_lib.h"


#define FIND_PROTO 1

int parse_ip(config_t* pi,
                           void *   value,
                           int offset,FILE* fp)
{
    char* v = value;/* 192.168.0.1 str to int  */
    char* ip_end;
    int ret = 0;
    struct in_addr ip;

    v = skip_space(v);
    ip_end = strchr(v,' ');
    if(!ip_end)
    {
      //research \n
      ip_end = strchr(v,'\n');
    }
    if(ip_end){
        *ip_end = 0;
        ret = inet_pton(AF_INET,v,(void*)&ip);        
        if(ret != 0)
            *(int*)((char*)pi+offset) = ip.s_addr;
        else 
        {
            *(int*)((char*)pi+offset) = 0;
        }
    }
    return 0;
}


int parse_proto(config_t* pi,
                           void *   value,
                           int offset,FILE* fp)
{
    char* v = value;
    
    v = skip_space(v);
    if(v){
        if(strstr(v,"TCP"))
        {
            *(char*)((char*)pi+offset) = 6;
            pi->flag |= FIND_PROTO;
        }
        else if(strstr(v,"UDP"))
        {
            *(char*)((char*)pi+offset) = 17;
            pi->flag |= FIND_PROTO;
        }
        else{
            pi->flag &= ~FIND_PROTO;
        }
    }
    return 0;
}


char* skip_space(char* s)
{
    while(s)
    {
        if((' '  == s[0])||('\t' == s[0]))
            s++;
        else
            break;
    }
    return s;
}

int parse_str(config_t* pi,
                           void *   value,
                           int offset,FILE* fp)
{
    char* v = value;
    int ret = 0;
    
    v = skip_space(v);
    if(v){
    
        strncpy(((char*)pi+offset),v,32);
    }
    return 0;
}

int parse_port(config_t* pi,
                           void *   value,
                           int offset,FILE* fp)
{
    char* v = value;
    int ret = 0;
    
    v = skip_space(v);
    if(v){
        ret = atoi(v);
    }
    *(short*)((char*)pi+offset) = htons(ret);
    return 0;
}
int parse_int(config_t* pi,
                           void *   value,
                           int offset,FILE* fp)
{
    char* v = value;
    int ret = 0;
    
    v = skip_space(v);
    if(v){
        ret = atoi(v);
    }
    *(int*)((char*)pi+offset) = (ret);
    return 0;
}

int is_comment_line(char* line)
{
    if(line[0] == '#')
        return 1;
        
    return 0;
}
