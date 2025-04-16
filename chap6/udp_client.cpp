#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdlib>

#define BUF_SIZE 30

void ErrorHandling(const char* message) {
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    socklen_t adr_sz;
    sockaddr_in serv_adr, from_adr;

    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <port>" << std::endl;
        return 1;
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        ErrorHandling("socket() error!");
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    if(serv_adr.sin_addr.s_addr == INADDR_NONE)
        ErrorHandling("inet_addr() error!");
    serv_adr.sin_port = htons(atoi(argv[2]));

    while(1) {
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        str_len = strlen(message);
        adr_sz = sizeof(serv_adr);
        sendto(sock, message, str_len, 0,
                (sockaddr*)& serv_adr, adr_sz);

        adr_sz = sizeof(from_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0,
                    (sockaddr*)& from_adr, &adr_sz);
        message[str_len] = 0;
        std::cout << "Message from server: " << message;
    }

    close(sock);
    return 0;
}