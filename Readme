# Sockslib (C++)

This is a mostly a proof of concept library but it works fine enough that it could be used in production.
This library currently only supports SOCKS5 proxies

## Usage

#### Example: Getting a web page
```cpp
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

    // Get ip from a web page
    sock.sendString("GET /raw HTTP/1.1\r\nHost: myexternalip.com\r\n\r\n");
    JUtils::DataSeg* seg = sock.recvSock(1024);
    std::cout << std::string(seg->content, seg->length);
    free(seg);

    return 0;
}
```

#### Connecting Guide
To connect to using this libary, you must first set a proxy using the `SocksSocket::setProxy` method, this method currently only supports IPv4 proxies.
```cpp
JUtils::SocksSocket sock = JUtils::SocksSocket();
sock.setProxy("127.0.0.1", 9050); // Set proxy
```
Once a proxy has been selected, you can connect using the `SocksSocket::connectSock` method. This method supports IPv4, IPv6 and Domain's. To use this method, you must provide a `JUtils::Socks5Address` with the address and address type defined.
```cpp
...
JUtils::Socks5Address addr("Somedomain.com", JUtils::Socks5AddrType::Domain); // Domain
JUtils::Socks5Address addr("1.2.3.4", JUtils::Socks5AddrType::IPv4); // IPv4
JUtils::Socks5Address addr("::", JUtils::Socks5AddrType::IPv6); // IPv6
sock.connectSock(addr, 80); // Connect to addr, port
```
#### Communication
Communication via through the proxy can be achieved via a number of methods, one method is using the built in `SocksSocket::sendSock` or `SocksSocket::sendString` methods. If you want lower level access to the socket, you can get the pointer using `SocksSocket::getSockObj`
```cpp
...
sock.sendString("Hello, World!"); // Send a NULL terminated string
sock.sendSock("Sockets are cool!", 17); // Send a char array with a fixed size - Not null terminated

int sock = sock.getSockObj(); // Get socket object so you can use low level socket calls (send, recv, etc)
```
To recieve data, you can use the `SocksSocket::recvSock` method.
```cpp
...
JUtils::DataSeg* seg = sock.recvSock(1024);                   // Recieve into a buffer of size 1024
std::cout << "Recieved Length: " << seg->length << std::endl; // Print the length of the recieved data
std::cout << std::string(seg->content, seg->length);          // Convert the recieved data to a string and print
```
