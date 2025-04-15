#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>  // for memset

#define BUF_SIZE 1024
#define OPSZ 4
#define RLT_SIZE 4

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
        ErrorHandling("socket() error!");
    
    sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        ErrorHandling("connect() error");
    else    
        std::cout << "Connected..." << std::endl;

    int input_nums;
    char message[BUF_SIZE] = {0};

    std::cout << "Operator counts: ";
    std::cin >> input_nums;
    message[0] = (char)input_nums;

    for (int i = 0; i < input_nums; i++) {
        std::cout << "Operand " << i + 1 << ": ";
        int num;
        std::cin >> num;
        memcpy(&message[i * OPSZ + 1], &num, sizeof(int));  // 正确存储int
    }

    std::cin.ignore();  // 清除输入缓冲区
    std::cout << "Operator (+, -, *, /): ";
    char op;
    std::cin >> op;
    message[input_nums * OPSZ + 1] = op;

    // 发送整个消息
    write(sock, message, input_nums * OPSZ + 2);

    // 接收结果
    int result;
    read(sock, &result, sizeof(int));

    std::cout << "Operation result: " << result << std::endl;
    close(sock);
    return 0;
}