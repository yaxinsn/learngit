//ffffffffffffff


#include <iostream>
#include "myXmlRpcServerMethods.h"
#include "XmlRpc.h"

class myXmlRpcServer {
public:
        myXmlRpcServer();
        void run();
private: 
        void pm_registerMethods();
        XmlRpc::XmlRpcServer pm_xmlRpcServer;
        std::list< myXmlRpcServerMethod* > pm_serverMethods;
};
