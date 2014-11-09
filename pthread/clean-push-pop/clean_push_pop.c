
/*


编译的命令：gcc -o clean -lpthread clean_push_pop.c
*/

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
void clean_fun1(void * arg)
{
	printf("%s:%d  this is clean fun1\n",__func__,__LINE__);
}
void clean_fun2(void * arg)
{
	printf("%s:%d  this is clean fun2\n",__func__,__LINE__);
}
void * thread_fun(void * arg)
{
	pthread_cleanup_push(clean_fun1,NULL);
	pthread_cleanup_push(clean_fun2,NULL);
	#if 0
	sleep(1);    //当sleep 1时，此线程会支持到函数的末尾。
	#else
	sleep(100);  //当sleep100时，此线程会被cancel掉（这是异常终止），则会调用clean_fun1 clean_fun2.
	#endif
	//pthread_exit(0);
	//return;
	pthread_cleanup_pop(0);  /* 参数0，表示执行此语句时，只把“清理函数”出栈，不执行“清理函数” */
	pthread_cleanup_pop(1);	 /* 参数!0, 表示执行此语句时，把“清理函数”出栈， 并执行“清理函数” */
	
	return NULL;
}
int main()
{
	pthread_t tid1;
	int err;
	err=pthread_create(&tid1,NULL,thread_fun,NULL);
	if(err!=0)
	{
		perror("pthread_create");
		exit(0);
	}
	sleep(3);
	//printf("test\n");
	err=pthread_cancel(tid1);
	if(err!=0)
	{
		perror("cancel error:");
		exit(0);
	}
	err=pthread_join(tid1,NULL);
	if(err!=0)
	{
		perror("pthread_join error:");
		exit(0);
	}

	printf("%s:%d main will exit \n",__func__,__LINE__);
	return 0;
}