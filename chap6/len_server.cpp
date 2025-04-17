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
    sockaddr_in my_adr, your_adr;

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock == -1)
        ErrorHandling("socket() error!");
    
    memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr*)& my_adr, sizeof(my_adr)) == -1)
        ErrorHandling("bind() error!");

    for(int i = 0; i < 2; i++){
        sleep(5);
        adr_sz = sizeof(your_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0,
                    (struct sockaddr*)& your_adr, &adr_sz);

        printf("message %d : %s \n", i + 1, message);
    }
    
    // 实际不会执行到这里
    close(sock);
    return 0;
}