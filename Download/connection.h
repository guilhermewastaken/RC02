#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "constants.h"

//Opens a socket and saves file descriptor to socketFD
//Returns 0 if no errors, 1 otherwise
int openSocket(char *ip, int port, int *socketFD);

//Reads and prints a normal request response
//Returns 0 if no errors, 1 otherwise
int readResponse(FILE* socketFile);

//Builds a request to send to the server
//Returns 0 if no errors, 1 otherwise
int writeRequest(REQUEST_TYPE type, char* argument, char* result);

//Writes a simple request to the ftp server
//Returns 0 if no errors, 1 otherwise
int sendRequest(int socketFD, char * request);

//Reads response to pasv command and setups up passive mode
//Returns 0 if no errors, 1 otherwise
int setupPasv(char *ip, int *port, FILE* socketFile);

#endif //PROTOCOL_H
