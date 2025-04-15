#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void ErrorHandling(const char* message) {
    std::cerr << message << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {

    int clnt_sock;
    char message[BUF_SIZE];
    int str_len;

    int recv_len, recv_cnt;

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
    else    
        puts("Connect.....................");

    while(1){
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        
        // write(sock, message, strlen(message));
        // str_len = read(sock, message, BUF_SIZE - 1);

        // improve--------------------------
        str_len = write(sock, message, strlen(message));

        recv_len = 0;
        while(recv_len < recv_cnt){        // means: size of recursive recieve data should equal to which transfered before
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
            if(recv_cnt == -1)
                ErrorHandling("read() error");
            recv_len += recv_cnt;
        }


        message[str_len] = 0;
        printf("Message from server: %s", message);
    }
    close(sock);
    return 0;
}
