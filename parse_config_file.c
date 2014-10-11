/***********************************************



************************************************/

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

#include "libkms.h"

#define  errf    printf

#define IP_FMT(__ip1) __ip1 & 0xFF, 	\
				__ip1 >> 8 & 0xFF, 	\
				__ip1 >> 16 & 0xFF, 	\
				__ip1 >> 24 & 0xFF

typedef struct lbg_service
{
    __u32    ip;  /* must network byte order ,  dip */
     __u8    weight;
    struct lbg_service* next;
    
}lbg_service_t;
#if 0
typedef struct kms_lbg_service
{
    uint32_t    ip;  /* must network byte order ,  dip */
    uint8_t     weight;  
    
}_kms_lbg_service_t;

#endif
typedef struct lbg_rule{
    __u32           sip_min;/* key info */
    __u32           sip_max;
    __u16           port;
    __u8            protocol;
    
    __u32           table_type;/* service info */
    int             server_num;
    lbg_service_t*  head;
    lbg_service_t*  end;
    int flag;

}lbg_rule_t;

typedef int (*peer_parse_handle)(lbg_rule_t* pi,
                           void *   value,
                           int offset, FILE* fp);
                           
 //typedef int (*peer_parse_handle)(lbg_rule* ,
 //                          void *   ,
 //                          int , FILE* );                          
typedef struct peer_parse_element
{
    char* key;
    peer_parse_handle handler;
    int offset;
}ppe_st;


#define FIND_MAXIP          1
#define FIND_MINIP          2
#define FIND_PORT           4
#define FIND_PROTO          8
#define FIND_TABLE_TYPE     16

#define FIND_SUCCESS     (FIND_PORT|FIND_PROTO|FIND_TABLE_TYPE)



#define offsetof(TYPE,MEMBER) ((int) &((TYPE*)0)->MEMBER)
#define peer_info_offsetof(A)   offsetof(struct lbg_rule,A)

char* skip_space(char* s);

int parse_port(lbg_rule_t* pi,
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

int parse_ip(lbg_rule_t* pi,
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


int parse_proto(lbg_rule_t* pi,
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

int parse_table(lbg_rule_t* pi,
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
int parse_svr(lbg_rule_t* pi,
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

ppe_st _keys[] = {
    {"maxip",        parse_ip,     peer_info_offsetof(sip_max)},
    {"minip",        parse_ip,     peer_info_offsetof(sip_min)},
    {"proto",        parse_proto,  peer_info_offsetof(protocol)},
    {"port",         parse_port,   peer_info_offsetof(port)},
    {"table",        parse_table,  peer_info_offsetof(table_type)},
    {"service",      parse_svr,    0},
};


void _usage(void)
{
    printf("Usage: lbgset -f cfgfile \n");
    exit(1);
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
int is_comment_line(char* line)
{
    if(line[0] == '#')
        return 1;
        
    return 0;
}
int is_rule_block(char* line)
{
      if(strstr(line,"rule{")){
        return 1;
        }
      else
      return 0;
}
int show_lbg_rule(lbg_rule_t* lrule)
{
    int i;
    lbg_service_t*  head = lrule->head;
    lbg_service_t* p = head;
    int num=0;
    int ret = 0;
    
    printf("rule info: port %d proto %d max ip [%d.%d.%d.%d] minip [%d.%d.%d.%d] table %d server_num %d \n",
        ntohs(lrule->port), lrule->protocol,
        IP_FMT(lrule->sip_max),
        IP_FMT(lrule->sip_min),
        lrule->table_type,
        lrule->server_num);   
    for(i = 0;i < lrule->server_num; i++)
    {
        printf("service ip %x, weight %d \n",
        p->ip,p->weight);
        p = p->next;
        num++;
    }
    printf("real num %d \n\n",num);
    
    return ret;
}
int set_lbg_rule(lbg_rule_t* lrule)
{
    struct kms_lbg_service* lsvr;
    int i;
    lbg_service_t* p = lrule->head;
    int num = 0;
    int ret;
    lsvr = malloc(sizeof(struct kms_lbg_service)*lrule->server_num);
    if(!lsvr){
        errf("set_lbg_rule: malloc %d mem fail!\n",sizeof(struct kms_lbg_service)*lrule->server_num);
        return -1;
    }
   
    for(i = 0;((i < lrule->server_num)&&(p)); i++)
    {
        lsvr[i].ip = p->ip;
        lsvr[i].weight = p->weight;
        p = p->next;
        num++;
    }

    ret = kms_lbc_set_service(lsvr,num,lrule->sip_min,lrule->sip_max,lrule->protocol,lrule->port,lrule->table_type);
    if(ret)
        errf("set_lbg_rule:call kms set lbg service fail!\n");
    free(lsvr);
    return ret;
}

void free_rule_services(lbg_rule_t* lrule)
{
    lbg_service_t* p = lrule->head;
    lbg_service_t* n;
    while(p){
        n = p->next;
        free(p);
        p = n;
    }
    lrule->head = lrule->end = NULL;
}
int parse_rule_block(FILE* fp)
{
    char line[1024] = {0};
    char* r;
    int i;
    lbg_rule_t lrule;
    int ret;
    char* p;

    memset(&lrule,0,sizeof(lbg_rule_t));

    lrule.sip_max = 0xffffffff;
    lrule.sip_min = 0;

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
                    _keys[i].handler(&lrule,(void*)p,_keys[i].offset,fp);
                }
            }
        }
    }  

    if(lrule.flag & FIND_SUCCESS)
    {
        
        //OK 
        //call libkms set this one.
        show_lbg_rule(&lrule);
       // ret = set_lbg_rule(&lrule);
        
    }
    else
    {
        errf("%s:the rule config is not integrated!\n",__func__);
        ret = -5;
    }
    free_rule_services(&lrule);
    return ret;
}

int read_config_and_set_rule(FILE* fp)
{
    char line[1024] = {0};
    char* r;
    while ((r = fgets(line,1023,fp)))
    {
        r = skip_space(r);
        if(is_comment_line(r))
            continue;
        if(is_rule_block(r))
            parse_rule_block(fp);
    }
    return 0;
}


int main(int argc,char* argv[])
{
    int c;
    char* cfgfile = NULL;
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
    read_config_and_set_rule(fp);

   fclose(fp);
   return 0;
}


