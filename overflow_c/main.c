#include <netinet/in.h> // sockaddr_in
#include <stdio.h>      // puts
#include <stdlib.h>     // exit
#include <unistd.h>     // read
#include <string.h>

#define PORT    8081
#define BUF_LEN 1024
#define CONTENT_LEN "Content-Length: "

int create_server();
int get_header_end(char* buffer, int einde);
int get_multipart_start(char* buffer, int einde);
int get_multipart_end(char* buffer, int einde);
int main_multipart(void);
int main_malloc(void);

int main(void) {
    /* main_multipart(); */
    main_malloc();
}


int main_malloc(void){
    int fd_server, fd_client;
    char buffer[BUF_LEN] = { 0 };

    fd_server = create_server();

    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    puts("Listening...");
    int result  = fd_client = accept(fd_server, (struct sockaddr*)&address, &addrlen);
    if (result < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    puts("---\tHeb client\t---");

    // headers
    int len = read(fd_client, buffer, BUF_LEN - 2);
    buffer[BUF_LEN - 1] = 0;
    int einde = get_header_end(buffer, len);
    buffer[einde] = 0;


    // parse content length
    const char* content_lenth_str = strstr(buffer, CONTENT_LEN) + strlen(CONTENT_LEN);
    *strstr(content_lenth_str, "\n") = '\0';
    int content_len = atoi(content_lenth_str);

    printf("content-len: %d\n", content_len);

    char* heap_buffer = malloc(content_len);

    printf("---\tEinde: %d\t---\n", einde);
    printf("---\tLen %d\t\t---\n", len);

    close(fd_client);
    close(fd_server);

    return 0;
}

int main_multipart(void) {
    int fd_server, fd_client;
    char buffer[BUF_LEN] = { 0 };

    fd_server = create_server();

    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    puts("Listening...");
    int result  = fd_client = accept(fd_server, (struct sockaddr*)&address, &addrlen);
    if (result < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    puts("---\tHeb client\t---");

    /* while (1) { */

    // headers
    int len = read(fd_client, buffer, BUF_LEN - 2);
    buffer[BUF_LEN -1] = 0;
    puts(buffer);
    int einde = get_header_end(buffer, len);
    memset(&buffer, 0, BUF_LEN);

    // content
    while (1) {
        int len = read(fd_client, buffer, BUF_LEN - 2);
        if (len == 0) {
            break;
        }
        buffer[BUF_LEN -1] = 0;

        int multipart_start = get_multipart_start(buffer, len);
        int multipart_einde = get_multipart_end(buffer, len);

        printf("---\tGelezen: %d\t\t---\n", len);
        printf("---\tMultipart start: %d\t---\n", multipart_start);
        printf("---\tMultipart einde: %d\t---\n", multipart_einde);
        printf("---\tMultipart len: %d\t---\n", len - multipart_einde);

        memset(&buffer[multipart_einde], 0,len - multipart_einde);
        puts(&buffer[multipart_start]);

        memset(&buffer, 0, BUF_LEN);
    }

    puts("---\tEinde request\t---");
    return 0;
}

// geeft het laatste charakter terug (niet inclusief!)
// zou in principe altijd naar een index naar '\r' terug geven
int get_header_end(char* buffer, int einde){
    if (einde <= 4 ) {
        return -1;
    }
    int index = einde;
    while(1) {
        if (index == 0) {
            return -1;
        } else if (buffer[index] == '\r' && buffer[index + 1] == '\n' &&
                buffer[index + 2] == '\r' && buffer[index + 3] == '\n'){
            return index;
        }
        index--;
    }
    return index;
}

int get_multipart_end(char* buffer, int einde){
    if (einde <= 6 ) {
        return -1;
    }
    int index = einde - 5;
    while(1) {
        if (index == 0) {
            return -1;
        } else if (buffer[index] == '-' && buffer[index + 1] == '-' &&
                buffer[index + 2] == '-' && buffer[index + 3] == '-' &&
                buffer[index + 4] == '-' && buffer[index + 5] == '-'){
            return index - 2;
        }
        index--;
    }
    return index;
}



int get_multipart_start(char* buffer, int einde){
    if (einde <= 4 ) {
        return 0;
    }
    int index = 0;
    while(1) {
        if (index == einde - 3) {
            return -1;
        } else if (buffer[index] == '\r' && buffer[index + 1] == '\n' &&
                buffer[index + 2] == '\r' && buffer[index + 3] == '\n'){
            return index + 2;
        }
        index++;
    }
    return 0;
}

// exit(1) als het niet werkt
int create_server() {
    int fd_server;
    if ((fd_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("socket failed");
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
