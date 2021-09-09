#pragma once

#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

namespace JUtils
{
    struct DataSeg
    {
        char* content;
        int length;
    };

    class Socket
    {
        int sock;
        bool connected;
    public:
        Socket();
        void connectSock(char* address, int port);
        void sendSock(char* data, int length);
        void sendString(char* str);
        DataSeg* recvSock(int length);
        int getSockObj();
    };
}