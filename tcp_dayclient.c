/*
 * Network computing
 * 		Assignment 1: A daytime TCP client pprogram (ver1)
 *      Written by Tran Quoc Hoan
 *		Last edited on April, 25, 2014
 *
 * Compatible with both IPv4 and IPv6
 * Perform rough check for IPversion
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define MAXBUF 4096
const int DAYTIME_PORT = 13;

/*
* ip_version
* 	rough check version of ip
* 	return 2 if length of hostname >= 40 
* 	return 1 if there is colon : in hostname (ipv6)
* 	else return 0 (ipv4)
*/
int ip_version( char* hostname ) {
	char colon = ':';
	int n = strlen(hostname);
	if (n >= 40) return 2;
	int i;
	for ( i = 0; i < n; i++ ) {
		if ( hostname[i] == colon ) return 1;
	}
	return 0;
}

/*
* main function
*/
int main( int argc, char * argv[] ) {
  
  if ( argc != 2 ) {
    fprintf( stderr, "Usage: %s <IPaddress>\n" , argv[0]);
    return 0;
  }
  
  int sockfd, confd, nbytes;
  char buf[ MAXBUF + 1];
  struct sockaddr_in server;
  struct sockaddr_in6 server6;
  int port = DAYTIME_PORT;
  
  int ip_ver = ip_version( argv[1] );
  if ( ip_ver == 2 ) {
	  perror("Format of ip address is not true (must be IPv4 or IPv6)");
	  return 0;
  }
  if ( ip_ver == 0) {  // IPv4
  	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
    if ( sockfd < 0 ) {
      perror("Socket v4 error");
	  close( sockfd );
      return 0;
    }
	
	// inet_pton
    bzero( &server, sizeof( server ) );
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
	
    if (  inet_pton( AF_INET, argv[1], &server.sin_addr ) <= 0 ) {
      fprintf( stderr, "inet_pton v4 error for %s\n", argv[1] );
      return 0;
    }
	
	 confd = connect( sockfd, (struct sockaddr *) &server, sizeof( server )); 
	
  } // finish IPv4 
  else {	// IPv6
	sockfd = socket( AF_INET6, SOCK_STREAM, 0);  
    if ( sockfd < 0 ) {
      perror("Socket v6 error");
	  close( sockfd );
      return 0;
    }
	
	// inet_pton
    bzero( &server6, sizeof( server6 ) );
    server6.sin6_family = AF_INET6;
    server6.sin6_port = htons( port );
	
    if (  inet_pton( AF_INET6, argv[1], &server6.sin6_addr ) <= 0 ) {
      fprintf( stderr, "inet_pton v6 error for %s\n", argv[1] );
      return 0;
    }
	confd = connect( sockfd, (struct sockaddr *) &server6, sizeof( server6 )); 
  } // finish IPv6
  
  if ( confd < 0 ) {
    perror("Connect error");
    return 0;
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
  close( confd );
  exit(1);
}
