# Client & Server Daytime Service in C/C++

This programming assignment consists of two simple program: a client and a server with daytime service (TCP)

- The user must be able to specify IP Address (compatible with both IPv4 and IPv6)
- The server program must reply with system time
- Perform error checking (ex. perror,...)

Updated in 2014/04/25: 
- tcp_dayclient.c (rough check IP version and connect to the server with appropriate protocol )
- tcp_dayclient_ver2.c ( using struct addrinfo )