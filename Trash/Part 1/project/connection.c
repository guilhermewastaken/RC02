#include "connection.h"

int openSocket(char ip[], int port, int *socketFD){
    struct	sockaddr_in server_addr;

    /*server address handling*/
    bzero((char*)&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);	/*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(port);		/*server TCP port must be network byte ordered */

    /*opens a TCP socket*/
    if ((*socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return 1;
    }

    /*connects to the server*/
    if(connect(*socketFD, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("connect()");
        return 1;
    }
    printf("\nOpening Socket\n\n");
    return 0;
}

int readResponse(FILE* socketFile){
    if (sleep(LATENCY) != 0)
        return 1;
    char * response = NULL; //Not casting to NULL resulted in random segmentation faults
    size_t bytesRead;
    printf("Response\n");
    while (TRUE){
        getline(&response, &bytesRead, socketFile);
        printf("%s", response);
        if (response[3] == ' ') { //End of message, doesn't have [code]-
            int code = strtol(response, &response, 10); //Line only has the response code
            if (code == 550 || code == 530)
                return 1;
            printf("\n");
            break;
        }
    }
    return 0;
}

int writeRequest(REQUEST_TYPE type, char* argument, char* result) {
    switch (type) {
        case USERNAME:
            if (sprintf(result, "user %s\r\n", argument) < 0)
                return 1;
            return 0;
        case PASSWORD:
            if (sprintf(result, "pass %s\r\n", argument) < 0)
                return 1;
            return 0;
        case PASV:
            strcpy(result, "pasv\r\n");
            return 0;
        case RETR:
            if (sprintf(result, "retr %s\r\n", argument) < 0)
                return 1;
            return 0;
    }
    return 1;
}

int sendRequest(int socket, char * request){
    int status = send(socket, request, strlen(request), 0);
    printf("Request\n%s\n", request);
    if (status == 0 || status == 1)
        return 1;
    return 0;
}

int setupPasv(char *ip, int *port, FILE* socketFile){
    if (sleep(LATENCY) != 0)
        return 1;
    char * response = NULL; //Not casting to NULL resulted in random segmentation faults
    size_t bytesRead = 0;

    while (TRUE){
        getline(&response, &bytesRead, socketFile);
        printf("%s", response);
        if (response[3] == ' '){
            break;
        }
    }
    //227 Entering Passive Mode (193,137,29,15,221,91).
    char p1[BUFFER_SIZE];
    char p2[BUFFER_SIZE];
    char *tmp = response;
    int curOcc = 1;
    while((tmp = strchr(tmp, ',')) != NULL) {
        if (curOcc != 4)
            *tmp++ = '.';   //Replace , with .
        else
            *tmp++ = '|';
        curOcc++;
    }
    char discard[BUFFER_SIZE];
    if (sscanf(response, "%[^(](%[^|]|%[^.].%[^)]).", discard, ip, p1, p2) != 4)
        return 1;
    *port = atoi(p1)*256 + atoi(p2);
    return 0;
}
