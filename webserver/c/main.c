#include <netinet/in.h> // sockaddr_in
#include <stdio.h>      // puts
#include <stdlib.h>     // exit
#include <unistd.h>     // read
#include <string.h>
#include <stdbool.h>

#define PORT        8081
#define BUF_LEN     1024
#define BEARER      "Bearer "
#define BEARER_LEN  100


int send_ok(int fd){
    char* response = "HTTP/1.1 200 OK\r\n\r\n";
    int resp = write(fd, response, strlen(response));
    /* printf("%d\n",resp); */
    return 0;
}

int send_unauthorized(int fd){
    char* response = "HTTP/1.1 401 Unauthorized\r\n\r\n";
    int resp = write(fd, response, strlen(response));
    /* printf("%d\n",resp); */
    return 0;
}

bool valideer_token(char* token){
    return false;
}

// exit(1) als het niet werkt
int create_server() {
    int fd_server;
    if ((fd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("socket failed");
        exit(1);
    }

    if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
        puts("setsockopt(SO_REUSEADDR) failed");
        exit(1);
    }

    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    if (bind(fd_server, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        puts("bind failed");
        exit(1);
    }
    if (listen(fd_server, 3) < 0) {
        puts("listen");
        exit(1);
    }
    return fd_server;
}

int accept_client(int fd_server){
    struct sockaddr_in address;
    int fd_client;
    socklen_t addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    puts("Listening...");
    int result = fd_client = accept(fd_server, (struct sockaddr*)&address, &addrlen);
    if (result < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    return fd_client;
}

int main_auth(void){
    int fd_server, fd_client;
    char buffer[BUF_LEN] = { 0 };

    // server socket maken
    fd_server = create_server();

    // client sockt accepteren
    fd_client = accept_client(fd_server);


    // request uitlezen
    int len = read(fd_client, buffer, BUF_LEN - 2);
    buffer[BUF_LEN - 1] = 0;

    // Bearer auth token er uit halen
    char* b_pointer = strstr(buffer, BEARER) + strlen(BEARER);
    *strstr(b_pointer, "\n") = '\0';
    /* puts(b_pointer); */

    bool is_authorized = false;
    char bearer_token[BEARER_LEN] = {0};

    // Bearer token kopieren
    memcpy(bearer_token, b_pointer, strlen(b_pointer));

    if(valideer_token(bearer_token)){
        is_authorized = true;
    }

    printf("%d\n", is_authorized);
    printf("%p\n", (void *) &is_authorized);
    printf("%p\n", (void *) &bearer_token[0]);
    printf("%p\n", (void *) &bearer_token[BEARER_LEN -1]);

    if (is_authorized == false){
        puts("Niet authorized\n");
        send_unauthorized(fd_client);
    } else {
        send_ok(fd_client);
        puts("Authorized\n");
    }

    close(fd_client);
    close(fd_server);
    return 0;
}

int main(void) {
    main_auth();
}
