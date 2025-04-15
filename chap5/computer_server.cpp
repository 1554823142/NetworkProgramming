#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#define BUF_SIZE 1024
#define OPSZ 4
#define RLT_SIZE 4

void ErrorHandling(const char* message) {
    std::cerr << message << std::endl;
    exit(1);
}

int compute(char message[]) {
    int input_num = (unsigned char)message[0];  // 避免符号扩展
    char op = message[input_num * OPSZ + 1];
    int result = 0;

    if (input_num == 0) return 0;

    // 读取第一个操作数
    memcpy(&result, &message[1], sizeof(int));

    for (int i = 1; i < input_num; i++) {
        int operand;
        memcpy(&operand, &message[i * OPSZ + 1], sizeof(int));
        switch (op) {
            case '+': result += operand; break;
            case '-': result -= operand; break;
            case '*': result *= operand; break;
            case '/':
                if (operand == 0) {
                    std::cerr << "Error: Division by zero!" << std::endl;
                    return 0;
                }
                result /= operand;
                break;
            default:
                std::cerr << "Error: Invalid operator!" << std::endl;
                return 0;
        }
    }
    return result;
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
        ErrorHandling("bind() error!");
    
    if (listen(serv_sock, 5) == -1)
        ErrorHandling("listen() error!");

    sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    while (true) {
        int clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
            ErrorHandling("accept() error!");

        char message[BUF_SIZE];
        read(clnt_sock, message, BUF_SIZE);

        int result = compute(message);
        write(clnt_sock, &result, sizeof(int));

        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}