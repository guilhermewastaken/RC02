#include "input-output.h"

int parseInput(char input[], Info *info) {
    //input format -> "ftp://[<user>:<password>@]<host>/<url-path>"

    //Validate format
    if (sscanf(input, "ftp://%s", input) != 1) {
        printf("Invalid Input, must start with 'ftp://':\n%s\n", input);
        return 1;
    }

    //Parse login credentials
    //@ is an illegal character in a path, therefore if this is executed we can be sure login credentials were provided
    if (sscanf(input, "%[^:]:%[^@]@%s", info->username, info->password, input) == 3) {
        info->containsLogin = TRUE;
    }
    else if (sscanf(input, "%[^@]@%s", info->username, input) == 2) { //No password provided
        info->containsLogin = TRUE;
        strcpy(info->password, "nopassword");
    }
    else {
        info->containsLogin = FALSE;
    }

    //Parse host URL and file path
    if (sscanf(input, "%[^/]/%s", info->host, info->filePath) != 2) {
        printf("Invalid Input, must end with '<host>/<path>':\n%s\n", input);
        return 1;
    }

    //Parse file name
    char *filename = strrchr(info->filePath, '/');
    if (filename == NULL)
        strcpy(info->fileName, info->filePath);
    else
        strcpy(info->fileName, filename+1);

    //Process URL
    if (getIP(info)) {
        return 1;
    }

    printf("Parsed Input\n");
    if (info->containsLogin) {
        printf("Username: '%s'\n", info->username);
        printf("Password: '%s'\n", info->password);
    }
    printf("Host URL: '%s'\n", info->host);
    printf("Host Name : %s\n", info->hostName);
    printf("IP Address : %s\n", info->ip);
    printf("File Path: '%s'\n", info->filePath);
    printf("File Name: '%s'\n", info->fileName);

    return 0;
}

int getIP(Info *info) {
    struct hostent *h;
    if ((h = gethostbyname(info->host)) == NULL) {
        herror("gethostbyname");
        return 1;
    }
    strcpy(info->hostName, h->h_name);
    strcpy(info->ip, inet_ntoa(*((struct in_addr *) h->h_addr)));
    return 0;
}

int writeFile(char* filename, int socketFD) {
    int fileFD = open(filename, O_WRONLY | O_CREAT, 0777);
    if (fileFD < 0)
        return 1;

    size_t bytes_read = 1;
    char response[1000];
    while (bytes_read > 0) {
        bytes_read = read(socketFD, response, 1000);
        if (bytes_read > 0) write(fileFD, response, bytes_read);
    }

    close(fileFD);
    return 0;
}