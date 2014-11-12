#include <iostream>
#include "XmlRpc.h"
using namespace XmlRpc;

int main(int argc, char* argv[])
{
        const char *server = "localhost";
        const int port = 8085;
        const char *uri = NULL;
        XmlRpcValue args, res;

        XmlRpcClient c( server, port, uri);
        c.execute("Add", args, res);
        std::cout<<"result is "<<res<<std::endl;
}
