//myXmlRpcServer.cpp


#include "myXmlRpcServer.h"

using namespace XmlRpc;
using namespace std;

myXmlRpcServer::myXmlRpcServer()
{
//call register methods
pm_registerMethods();

//set port bind and listen
int port = 8085;
pm_xmlRpcServer.bindAndListen(port);
std::cout<<"XmlRpcSever running in port "<<port<<std::endl;
}

void
myXmlRpcServer::pm_registerMethods()
{
        Add* a=new Add(&pm_xmlRpcServer);
        myXmlRpcServerMethod *p=a;
        pm_serverMethods.push_back(p); 
}

void
myXmlRpcServer::run()
{
        pm_xmlRpcServer.work(-1);
}

