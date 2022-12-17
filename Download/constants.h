#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BUFFER_SIZE 256
#define TRUE 1
#define FALSE 0
#define PORT 21
#define LATENCY 1
#define INPUT_START 6
typedef enum {
    USERNAME = 0,
    PASSWORD,
    PASV,
    RETR
} REQUEST_TYPE;

#endif //CONSTANTS_H
