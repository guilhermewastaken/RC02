#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "input-output.h"
#include "connection.h"

int main(int argc, char *argv[]) {
    Info info;
    if (argc != 2)
        return 1;
    if (parseInput(argv[1], &info))
        return 1;

    int socketFD;
    if (openSocket(info.ip,PORT, &socketFD) != 0)
        return 1;   //Initiate connection
    FILE* socketFile = fdopen(socketFD, "r");
    if (readResponse(socketFile))
        return 1;

    if (!info.containsLogin) {
        strcpy(info.username, "ftp");
        strcpy(info.password, "nopassword");
    }
    char request[BUFFER_SIZE];
    writeRequest(USERNAME, info.username, request);
    sendRequest(socketFD, request);
    if (readResponse(socketFile))
        return 1;
    writeRequest(PASSWORD, info.password, request);
    sendRequest(socketFD, request);
    if (readResponse(socketFile))
        return 1;

    int portPasv;
    writeRequest(PASV, NULL, request);
    sendRequest(socketFD, request);
    if (setupPasv(info.ip, &portPasv, socketFile))
        return 1;
    printf("IP: %s\nPort: %i\n", info.ip, portPasv);

    int socketFDReceive;
    if (openSocket(info.ip, portPasv, &socketFDReceive) != 0)
        return 1;
    writeRequest(RETR, info.filePath, request);
    sendRequest(socketFD, request);
    if (readResponse(socketFile) != 0)
        return 1;
    writeFile(info.fileName, socketFDReceive);

    return 0;
}