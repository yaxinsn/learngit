#include <stdio.h>
#include "libpim.h"

int main(void)
{
	im_ctrl_t* imc;
	imc = im_create();
	int i;
	if(imc == NULL)
		printf("im_create fail!\n");
	for(i=0;i<4;i++)
	{
		im_push_msg(imc,NULL,0);
		sleep(1);
	}
	im_destroy(imc);
	printf("I am main\n");
	return 0;
}
