#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>


#define BUF_SIZE 1024

void ErrorHandling(const char* message) {
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {

    int clnt_sock;
    char message[BUF_SIZE];
    int str_len;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        ErrorHandling("socket() error");

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        ErrorHandling("bind() error");

    if (listen(serv_sock, 5) == -1)
        ErrorHandling("listen() error");

    sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    
    for(int i = 0; i < 5; i++){
        clnt_sock = accept(serv_sock, (struct sockaddr*)& clnt_addr, &clnt_addr_size);
        if(clnt_sock == -1)
            ErrorHandling("accept() error!");
        else
            printf("Conneted client %d \n", i + 1);

        while((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
            write(clnt_sock, message, str_len);
        
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}
