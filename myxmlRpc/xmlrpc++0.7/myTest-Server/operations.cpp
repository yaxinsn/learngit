
//#include <std>
#include <iostream>
#include "operations.h"
//using namespace std;


int operations::add()
{
        std::cout << "Sum of "<<op1<<" + "<<op2<<" = "<<op1+op2<<std::endl;
        return(op1 + op2);
}
#if 1
operations::operations(int i, int j)
{
	op1 = i;
	op2 = j;
}

#endif

