#include "sockslib.hpp"

namespace JUtils
{
    SocksSocket::SocksSocket()
    {
        this->proxyPort = 0;
        this->proxyAddress = 0;
    }

    void SocksSocket::setProxy(char* address, int port)
    {
        this->proxyAddress = address;
        this->proxyPort = port;
    }

    int SocksSocket::handshake_v5(Socks5Address address, int port)
    {
        this->sendSock("\x05\x01\x00", 3); // Greet the server
        DataSeg* serverChoice = this->recvSock(2);

        if ((int)serverChoice->content[1] != 0x00) 
        {
            free(serverChoice);
            return 1;
        }
        free(serverChoice);
        
        this->sendSock("\x05\x01\x00", 3);                 // Send CCR packet

        DataSeg* addrFmt = address.formatAddress();        // Send address
        this->sendSock(addrFmt->content, addrFmt->length);
        free(addrFmt);
        
        char buf[2];                                       // Send Port
        buf[0] = port / 256;
        buf[1] = port % 256;
        this->sendSock(buf, 2);

        DataSeg* vsr = this->recvSock(3);                  // Get start of CCR response packet
        if ((int)vsr->content[1] != 0x00)
        {
            free(vsr);
            return 1;
        }
        free(vsr);

        // Please kill me
        DataSeg* addrtype = this->recvSock(1);             // Recieve address type
        switch ((int)addrtype->content[0])
        {
            case 0x01: {
                free(this->recvSock(6));
                break;
            }
            case 0x03: { // Case 0x03 Domain
                DataSeg* len = this->recvSock(1);                 // Recieve length of address
                free(this->recvSock(((int)len->content[0]) + 2)); // Recieve address, port and then free it
                free(len);                                        // Free address length var
                break;
            }
            case 0x04: {
                free(this->recvSock(18));
                break;
            }
        }
        
        return 0;
    }

    void SocksSocket::connectSock(Socks5Address address, int port)
    {
        // Check that the proxy has been set
        if (this->proxyPort == 0 || this->proxyAddress == 0)
        {
            return; // Faliure (Proxy Not Set)
        }

        Socket::connectSock(this->proxyAddress, this->proxyPort);

        handshake_v5(address, port);
    }

    void SocksSocket::connectSock(char* address, int port)
    {
        Socks5Address s5address(address, Socks5AddrType::IPv4);
        this->connectSock(s5address, port);
    }

    DataSeg* Socks5Address::formatAddress()
    {
        char prefix;
        char buf[255];
        int datalen = 0;

        memset(buf, 0, 255);

        switch (this->addrType)
        {
            case IPv4: {
                prefix = '\x01';
                inet_pton(AF_INET, this->address, buf);
                datalen = 4;

                break;
            }
            case IPv6: {
                prefix = '\x04';
                inet_pton(AF_INET6, this->address, buf);
                datalen = 16;

                break;
            }
            case Domain: {
                prefix = '\x03';
                datalen = strlen(this->address) + 1;

                buf[0] = (char)(datalen-1);

                for (int i = 1; i < datalen + 1; i++)
                    buf[i] = this->address[i-1];

                break;
            }
        }

        char* finalbuf = (char*) malloc(sizeof(char) * (datalen + 1));
        finalbuf[0] = prefix;

        for (int i = 1; i < datalen + 1; i++) 
            finalbuf[i] = buf[i-1];
        
        DataSeg* seg = (DataSeg*) malloc(sizeof(DataSeg) * (datalen + 1));
        seg->content = finalbuf;
        seg->length = datalen + 1;
        return seg;
    }

    Socks5Address::Socks5Address(char* address, Socks5AddrType type)
    {
        this->addrType = type;
        this->address = address;
    }
}   