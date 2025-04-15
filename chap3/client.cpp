#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void ErrorHandling(const char* message) {
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <IP> <port>" << std::endl;
        return 1;
    }

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        ErrorHandling("socket() error");

    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        ErrorHandling("connect() error");

    char message[100];
    int str_len = read(sock, message, sizeof(message) - 1);
    if (str_len == -1)
        ErrorHandling("read() error");

    message[str_len] = '\0';
    std::cout << "Message from server: " << message << std::endl;

    close(sock);
    return 0;
}
