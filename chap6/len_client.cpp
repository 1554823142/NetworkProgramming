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
    char message1[] = "Hello ";
    char message2[] = "How are";
    char message3[] = " you";

    int str_len;
    socklen_t your_adr_sz;
    sockaddr_in your_adr;

    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <port>" << std::endl;
        return 1;
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        ErrorHandling("socket() error!");
    
    memset(&your_adr, 0, sizeof(your_adr));
    your_adr.sin_family = AF_INET;
    your_adr.sin_addr.s_addr = inet_addr(argv[1]);
    if(your_adr.sin_addr.s_addr == INADDR_NONE)
        ErrorHandling("inet_addr() error!");
    your_adr.sin_port = htons(atoi(argv[2]));

    /// 发送这三条信息
    sendto(sock, message1, sizeof(message1), 0, 
            (struct sockaddr*)& your_adr, sizeof(your_adr));
    sendto(sock, message2, sizeof(message2), 0, 
        (struct sockaddr*)& your_adr, sizeof(your_adr));
    sendto(sock, message3, sizeof(message3), 0, 
        (struct sockaddr*)& your_adr, sizeof(your_adr));
    close(sock);
    return 0;
}