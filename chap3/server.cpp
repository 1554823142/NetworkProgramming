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

void ErrorHandling(const char* message) {
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
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
    int clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        ErrorHandling("accept() error");

    const char* message = "Hello from C++ Server!";
    write(clnt_sock, message, strlen(message));

    close(clnt_sock);
    close(serv_sock);
    return 0;
}
