#include "Sockets.hpp"

namespace JUtils
{
    Socket::Socket()
    {
        this->sock = socket(AF_INET, SOCK_STREAM, 0);
        this->connected = false;
    }

    int Socket::getSockObj()
    {
        return this->sock;
    }

    void Socket::connectSock(char* address, int port)
    {
        if (connected) return;

        struct sockaddr_in sockaddress;

        sockaddress.sin_family = AF_INET;
        sockaddress.sin_addr.s_addr = inet_addr(address);
        sockaddress.sin_port = htons( port );

        int result = connect(this->sock, (struct sockaddr *)&sockaddress, sizeof(sockaddress));
        if (result != -1)
        {
            connected = true;
        }
    }

    void Socket::sendSock(char* data, int length)
    {
        if (!connected) return;
        send(this->sock, data, length, 0);
    }

    void Socket::sendString(char* str)
    {
        if (!connected) return;
        this->sendSock(str, strlen(str));
    }

    DataSeg* Socket::recvSock(int length)
    {
        if (!connected) return 0;

        char* buf = (char*) malloc(sizeof(char) * length);
        memset(buf, 0, length);
        int bytes_recv = recv(this->sock, buf, length, 0);
        
        DataSeg* segment = (DataSeg*) malloc(sizeof(DataSeg));
        segment->content = buf;
        segment->length = bytes_recv;

        return segment;
    }

}