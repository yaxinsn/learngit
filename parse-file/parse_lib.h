

/******************************************
FILE: 		parse_lib.h
author:		yaxinsn
version:		1

desception:    parse strings   

key function:	
lib example:

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

typedef struct config
{
    unsigned int    ip;  /* must network byte order ,  dip */
    int    key_int;
    int    key_length;
    char   key_name[32];
    unsigned short port;
    unsigned char proto;//tcp 6 udp 17  

    int flag;    
}config_t;


/*函数指针的定义　　 */
typedef int (*peer_parse_handle)(config_t* pi,
                           void *   value,
                           int offset, FILE* fp);


typedef struct peer_parse_element
{
    char* key;
    peer_parse_handle handler;
    int offset;
}ppe_st;



/***************************************
 *
 *
 * ***************************************/
int parse_ip(config_t* pi,
                           void *   value,
                           int offset,FILE* fp);


int parse_proto(config_t* pi,
                           void *   value,
                           int offset,FILE* fp);

char* skip_space(char* s);
int parse_str(config_t* pi,
                           void *   value,
                           int offset,FILE* fp);
int parse_port(config_t* pi,
                           void *   value,
                           int offset,FILE* fp);
int parse_int(config_t* pi,
                           void *   value,
                           int offset,FILE* fp);



int is_comment_line(char* line);
