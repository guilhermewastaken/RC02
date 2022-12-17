#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "constants.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define h_addr h_addr_list[0]

typedef struct {
    int containsLogin;             //Indicates whether login credentials were provided
    char username[BUFFER_SIZE];     //Username, provided by user (optional)
    char password[BUFFER_SIZE];     //Password, provided by user (optional)
    char host[BUFFER_SIZE];         //Host URL
    char hostName[BUFFER_SIZE];     //Host name, obtained with DNS
    char ip[BUFFER_SIZE];           //Host IP, obtained with DNS
    char filePath[BUFFER_SIZE];     //File Path, provided by user
    char fileName[BUFFER_SIZE];     //File Name, obtained from filePath

} Info;

//Parses the user input and saves it to info struct
//Returns 0 if no errors, 1 otherwise
int parseInput(char input[], Info *info);

//Finds IP and host name from host URL
//Returns 0 if no errors, 1 otherwise
int getIP(Info *info);

//Write file from server reply
//Returns 0 if no erros, 1 otherwise
int writeFile(char* filename, int socketFD);
#endif //INPUT_H
