/*
 * Network computing
 * 		Assignment 1: A daytime TCP client pprogram (ver2)
 *      Written by Tran Quoc Hoan
 *		Last edited on April, 25, 2014
 *
 * Compatible with both IPv4 and IPv6
 * Using struct addrinfo (ver2)
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define MAXBUF 4096
const char *DAYTIME_PORT = "13";

/*
* connect to server using service
*/
int connect_server ( const char* servname, const char* port, int family, int socktype) {
	struct addrinfo servaddr;
	struct addrinfo *rcv;
	int tmp, sockfd;
	
	memset( &servaddr, 0, sizeof( struct addrinfo));
	servaddr.ai_family = family;
	servaddr.ai_socktype = socktype;
	tmp = getaddrinfo( servname, port, &servaddr, &rcv);
	if ( tmp < 0 ) {
		perror("getaddrinfo error");
		return -1;
	}
	
	sockfd = -1;
	while ( rcv ) {
		sockfd = socket( rcv->ai_family, rcv->ai_socktype, rcv->ai_protocol );
		if ( sockfd >= 0) {
			int confd = connect( sockfd, rcv->ai_addr, rcv->ai_addrlen);
			if ( confd == 0 ) break;
			close( sockfd );
			sockfd = -1;
		}
		// check next address family
		rcv = rcv->ai_next;
	}
	return sockfd;
}

/*
* main function
*/
int main( int argc, char * argv[] ) {
  
  if ( argc != 2 ) {
    fprintf( stderr, "Usage: %s <IPaddress>\n" , argv[0]);
    return 0;
  }
  
  int sockfd, nbytes;
  char buf[ MAXBUF + 1];
  
  sockfd = connect_server( argv[1], DAYTIME_PORT, AF_UNSPEC, SOCK_STREAM );
  if ( sockfd < 0 ) {
	  perror( "Connect error");
	  return -1;
  }
  
  
  memset( buf, 0, sizeof( buf ));
  // Read data from socket
  while ( ( nbytes = read( sockfd, buf, MAXBUF )) > 0) {
    buf[ nbytes ] = '\0';
    if ( fputs( buf, stdout ) == EOF ) {
      perror( "fputs error" );
      return 0;
    }
  }

  if ( nbytes < 0 ) {
    perror ( "read error" );
    return 0;
  }
  close( sockfd );
  exit(1);
}
