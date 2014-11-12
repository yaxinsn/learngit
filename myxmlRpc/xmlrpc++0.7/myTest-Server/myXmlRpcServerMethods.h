// 6 myXmlRpcServerMethods.h
#ifndef METHOD_H
#define METHOD_H
#include "XmlRpc.h"
#include "assert.h"
using namespace XmlRpc;
using namespace std;
class myXmlRpcServerMethod:public XmlRpcServerMethod
{
public:
 myXmlRpcServerMethod(const char *name, XmlRpcServer * server):XmlRpcServerMethod(name, server) {}
//   myXmlRpcServerMethod(const char *name, XmlRpcServer * server) {}
   virtual void execute(XmlRpcValue & params, XmlRpcValue& result) {assert(0);}
};

class Add:public myXmlRpcServerMethod
{
public:
   Add(XmlRpcServer* s);
   virtual void execute(XmlRpcValue& params, XmlRpcValue& result);
};

#endif
