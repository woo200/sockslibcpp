#pragma once

#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <string.h>

#include <iostream> // Debugging (Remove in release)

#include "Sockets.hpp"

namespace JUtils
{
    enum Socks5AddrType { IPv4, IPv6, Domain };

    class Socks5Address
    {
    public:
        Socks5Address(char* address, Socks5AddrType type);
        DataSeg* formatAddress();
    private:
        Socks5AddrType addrType;
        char* address;
    };

    class SocksSocket: public Socket
    {   
        int proxyPort;
        char* proxyAddress;

        int handshake_v5(Socks5Address address, int port);
    public:
        SocksSocket();

        void connectSock(Socks5Address address, int port);
        void connectSock(char* address, int port);
        void setProxy(char* address, int port);
    };
    
}