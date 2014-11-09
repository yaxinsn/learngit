
/*


��������gcc -o clean -lpthread clean_push_pop.c
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
	sleep(1);    //��sleep 1ʱ�����̻߳�֧�ֵ�������ĩβ��
	#else
	sleep(100);  //��sleep100ʱ�����̻߳ᱻcancel���������쳣��ֹ����������clean_fun1 clean_fun2.
	#endif
	//pthread_exit(0);
	//return;
	pthread_cleanup_pop(0);  /* ����0����ʾִ�д����ʱ��ֻ�ѡ�����������ջ����ִ�С��������� */
	pthread_cleanup_pop(1);	 /* ����!0, ��ʾִ�д����ʱ���ѡ�����������ջ�� ��ִ�С��������� */
	
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