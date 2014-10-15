
/******************************************
FILE: 		parse_config_file.c
author:		yaxinsn
version:		1

desception: this code read config file,and parse 'key = value'  

key function:	

lib example:	getopt_long,socket.sendto.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
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

/*
 * parse file content
 * key_ip = x.x.x.x
 * key_int = num
 * key_name = string
 * key_length = num
 * key_port = num
 * key_proto = TCP,UDP
 *
 * */


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

#define  errf    printf

#define IP_FMT(__ip1) __ip1 & 0xFF, 	\
				__ip1 >> 8 & 0xFF, 	\
				__ip1 >> 16 & 0xFF, 	\
				__ip1 >> 24 & 0xFF
                           
 //typedef int (*peer_parse_handle)(lbg_rule* ,
 //                          void *   ,
 //                          int , FILE* );                          



#define offsetof(TYPE,MEMBER) ((int) &((TYPE*)0)->MEMBER)
#define peer_info_offsetof(A)   offsetof(struct config,A)


#if 0
int parse_ip(config_t* pi,
                           void *   value,
                           int offset,FILE* fp)
{
    char* v = value;/* 192.168.0.1 str to int  */
    char* ip_end;
    int ret = 0;
    struct in_addr ip;

    v = skip_space(v);
    ip_end = strchr(v,';');
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
#endif
#if 0
int parse_table(config_t* pi,
                           void *   value,
                           int offset,FILE* fp)
{
    char* v = value;
    
    v = skip_space(v);
    if(v){
        if(strstr(v,"user"))
        {
            *(int*)((char*)pi+offset) = 1;
            pi->flag |= FIND_TABLE_TYPE;
        }
        else if(strstr(v,"default"))
        {
            *(int*)((char*)pi+offset) = 2;
            pi->flag |= FIND_TABLE_TYPE;
        }
        else
            pi->flag &= ~FIND_TABLE_TYPE;
    }
    return 0;
}
#endif
#if 0
int parse_svr(config_t* pi,
                           void *   value,
                           int offset, FILE* fp)
{
    char* v = value;
    char* ipend;
    int ret;
    struct in_addr ip;
    int weight = 0;
    lbg_service_t* newone;
    
    offset = offset;
    fp = fp;
    
    v = skip_space(v);
    ipend = strchr(v,',');
    if(ipend){
        *ipend = 0;
        ipend++;
        ret = inet_pton(AF_INET,v,(void*)&ip);/* host to net */
        if (ret < 0){
            return -1;
        }
        ipend[strlen(ipend)-1] = 0;
        ipend = skip_space(ipend);
        weight = atoi(ipend);
        if (weight <= 0){
            return -2;
        }
        if(weight > 255)
            weight = 255; 
    }
    else
        return -3;
        
    newone = malloc(sizeof(lbg_service_t));
    if(newone){
        newone->ip = (ip.s_addr);/* */
        newone->weight = weight;
        newone->next = NULL;
        
        pi->server_num++;
        
        if(pi->head == NULL){
            pi->head = newone;
            pi->end = newone;
        }
        else
        {
            pi->end->next = newone;
            pi->end = newone;
        }
    }
    return 0;
}
#endif

ppe_st _keys[] = {
    {"key_ip",        parse_ip,     peer_info_offsetof(ip)},
    {"key_proto",        parse_proto,  peer_info_offsetof(proto)},
    {"key_port",         parse_port,   peer_info_offsetof(port)},
    {"key_name",        parse_str,  peer_info_offsetof(key_name)},
    {"key_int",         parse_int,    peer_info_offsetof(key_int)},
    {"key_length",         parse_int,    peer_info_offsetof(key_length)},
};


void _usage(void)
{
    printf("Usage: parse_config -f cfgfile \n");
    exit(1);
}
int is_rule_block(char* line)
{
      if(strstr(line,"rule{")){
        return 1;
        }
      else
      return 0;
}


int parse_keys(FILE* fp,config_t* cfg)
{
    char line[1024] = {0};
    char* r;
    int i;
    int ret = 0;
    char* p;


    while ((r = fgets(line,1023,fp)))
    {
        r = skip_space(r);
        if(is_comment_line(r))
            continue;
        if(*r == '}'){
            break;
            }
        for (i = 0;i < sizeof(_keys)/sizeof(ppe_st);i++)
        {
            p = strstr(r, _keys[i].key);
            if (p)
            {
                p = strchr(p,'=');
                if(p){
                    p++;
                    _keys[i].handler(cfg,(void*)p,_keys[i].offset,fp);
                }
            }
        }
    }  

    return ret;
}

int main(int argc,char* argv[])
{
    int c;
    char* cfgfile = NULL;
    config_t cfg;


    FILE* fp;

        if(argc == 1){
            _usage();
        }
    while((c = getopt (argc,argv, "f:")) != -1){
        switch(c)
        {
        case 'f':
            cfgfile = optarg;
            goto next;
            break;
        default:  
            _usage();
        }
    }
    if(c == -1){
        printf("%d\n",__LINE__);
        _usage();
       
    }
next:

    /* open file */
    fp = fopen(cfgfile,"r");
    if(!fp){
        printf("Open %s fail! \n",cfgfile);
        perror("");
        exit(1);
    }
    parse_keys(fp,&cfg);

/************/
/*show the key*/
   printf("name <%s>\n",cfg.key_name);
   printf("key_int <%d>\n",cfg.key_int);
   printf("key_length <%d>\n",cfg.key_length);
   printf("port <%x>\n",htons(cfg.port));
   printf("ip <%x>\n",htonl(cfg.ip));



   fclose(fp);
   return 0;
}


