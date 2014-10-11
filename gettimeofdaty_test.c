#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

int main(void)
{

    int ret;
//  long  t;
//   FILE* fd;
    struct timeval _tv;
    struct timezone _tz; 
    struct tm* tm_a;
    int milsec = 0;

    ret = gettimeofday(&_tv,NULL);
    if(ret != 0)
    {
//   	    uail_logd(UAIL_LOG_CAT_ERROR, UAIL_LOG_LEV_MAJOR,
  printf(      "get time failed errno %d",errno); 
    }
    tm_a = gmtime(&_tv.tv_sec);
    milsec = _tv.tv_usec/1000 - _tv.tv_sec*1000;
    printf("aaaa/%02u.%u.%02u.%02u.%03u_.info",
        tm_a->tm_mday,tm_a->tm_hour,tm_a->tm_min,tm_a->tm_sec,milsec);
	printf("-------%3u\n------",milsec);
    //    0xff&(cs->ip>>8),0xff&(cs->ip),cs->authed);

}
