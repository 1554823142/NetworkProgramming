#include<stdio.h>
#include <arpa/inet.h>
#include <iostream>

void ErrorHandling(const char* message) {
    std::cerr << message << std::endl;
    exit(1);
}

int f1()
{
    char* addr = "1.2.3.4";
    char* addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr);
    if(conv_addr == INADDR_NONE)
        printf("Error!\n");
    else
        printf("Network ordered integer addr: %#1x \n", conv_addr);
    

    unsigned long conv_addr2 = inet_addr(addr2);
    if(conv_addr2 == INADDR_NONE)
        printf("Error!\n");
    else
        printf("Network ordered integer addr: %#1x \n\n", conv_addr2);

    return 0;
}


// inet_aton
int f2()
{
    char* addr = "127.232.124.79";
    struct sockaddr_in addr_inet;
    if(!inet_aton(addr, &addr_inet.sin_addr))
        ErrorHandling("conversion Error");
    else
        printf("NetWork ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);

    return 0;
}

int f3()
{
    struct sockaddr_in addr1, addr2;
    char* str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);
    printf("Dotted-Decimal notation: %s \n", str_ptr);

    inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notation2: %s \n", str_ptr);
    printf("Dotted-Decimal notation3: %s \n", str_arr);
    return 0;
}

int main()
{
    // f1();
    // f2();
    f3();
    return 0;
}