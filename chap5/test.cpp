#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

#define BUF_SIZE 1024

// 规定待运算的数与运算结果为4B
#define OPSZ 4
#define RLT_SIZE 4

int compute(char message[])
{
    int input_num = (int)(message[0]);
    char OP = message[input_num * OPSZ + 1];
    int result = 0;

    if(input_num == 0)  return result;

    switch(OP)
    {
        case '+':
            {
                for(int i = 0; i < input_num; i++)
                    result += (int)message[i * OPSZ + 1];
                break;
            }
        case '-':
            {result = (int)(message[1]);
            for(int i = 1; i < input_num; i++)
                result -= (int)message[i * OPSZ + 1];
            break;}
        case '*':
            {result = (int)(message[1]);
            for(int i = 1; i < input_num; i++)
                result *= (int)message[i * OPSZ + 1];
            break;}
        case '/':
            {int result = (int)(message[1]);
            for(int i = 0; i < input_num; i++){
                if((int)(message[i * OPSZ + 1] == 0))
                    std::cout << "error divide num" << std::endl;

                result /= (int)message[i * OPSZ + 1];
            }
            break;}
        default:
            std::cout << "error op" << std::endl;
            break;
    }
    return result;
}

int main(){
    int input_nums;
    char message[BUF_SIZE];
    std::cout << "Operator counts: ";
    std::cin >> input_nums;         // 也需要发送到服务端

    // protocol:
    // 1. 首先传递运算数的个数(1Bint)
    // 2. 传递 4B 的操作数



    // p1
    message[0] = char(input_nums);
    

    for(int i = 0; i < input_nums; i++){
        std::cout << "Operand " << i + 1 << ": ";

        // P2
        std::cin >> message[i * OPSZ + 1];            // 输入操作数

        // str_len = write(sock, message, );    // 发送操作数

        /// 不必每输入一个数就传递, 可以输入完全部的OP_num再传递
    }

    // transport the next operand

    // 接收OP
    fgetc(stdin);               // 输入字符前需要删除缓冲区中的\n

    fputs("Operator: ", stdout);

    // transport the operator
    std::cin >> message[input_nums * OPSZ + 1];

    int result = compute(message);
    std::cout << result << std::endl;

}