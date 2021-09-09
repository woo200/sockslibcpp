#include <stdlib.h>

#include "sockslib.hpp"

int main(void)
{
    // Create sock and set proxy
    JUtils::SocksSocket sock = JUtils::SocksSocket();
    sock.setProxy("127.0.0.1", 9050);

    // Connect to myexternalip.com:80
    JUtils::Socks5Address addr("myexternalip.com", JUtils::Socks5AddrType::Domain);
    sock.connectSock(addr, 80);

    // Get a my ip from a web page
    sock.sendString("GET /raw HTTP/1.1\r\nHost: myexternalip.com\r\n\r\n");
    JUtils::DataSeg* seg = sock.recvSock(1024);                   // Recieve into a buffer of size 1024
    std::cout << "Recieved Length: " << seg->length << std::endl; // Print the length of the recieved data
    std::cout << std::string(seg->content, seg->length);          // Convert the recieved data to a string and print

    return 0;
}