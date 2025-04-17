# UDP

与TCP不同, UDP不会进行流控制, 无需经过连接过程, 只有创建套接字和数据交换

<img src="./assets/image-20250416222147211.png" alt="image-20250416222147211" style="zoom:67%;" />

UDP最重要的就是**根据端口号**传到主机的数据包交付到最终的UDP套接字

TCP慢于TCP的原因:

- 收发数据前后进行的链接设置及清除过程
- 接发数据中为保证可靠性而添加的流控制

如果收发的**数据量小但需要频繁连接**时UDP比TCP更高效



## 套接字关系

TCP中, 套接字之间一一对应, 如果要向10个客户端提供服务, 则除守卫之外还要10个服务端套接字

UDP中客户端和服务端仅需要1个即可, 就像收发信件的邮筒各方仅需要1个即可. **只需要1个UDP套接字就可以向任意的主机传递数据, 一个UDP套接字可以和多台主机通信**

<img src="./assets/image-20250416222759311.png" alt="image-20250416222759311" style="zoom:67%;" />

## UDP I/O函数

TCP创建连接就无需添加地址信息, UDP每次传输数据树妖添加目标地址信息(寄信时需要加上地址)



传输数据:

```c++
ssize_t sendto (int sock, const void * buff, size_t nbytes, int flags, const struct sockaddr * to, socklen_t addrlen);
```

- `to`: 存有目标地址信息的`sockaddr`结构体变量的地址值 

这也是与TCP输出函数最大的区别: 此函数需要向他**传递目标地址的信息**



接收数据:

```c++
ssize_t recvfrom (int sock, void *__restrict buff, size_t nbytes, int flags, struct sockaddr *__restrict from, socklen_t *__restrict addrlen);
```

实例:

- 客户端:

  ```c++
  serv_sock = socket(PF_INET, SOCK_DGRAM, 0);				// SOCK_DGRAM
  if(serv_sock == -1)
      ErrorHandling("socket() error!");
  
  memset(&serv_adr, 0, sizeof(serv_adr));
  serv_adr.sin_family = AF_INET;
  serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_adr.sin_port = htons(atoi(argv[1]));
  
  if(bind(serv_sock, (struct sockaddr*)& serv_adr, sizeof(serv_adr)) == -1)
      ErrorHandling("bind() error!");
  
  
  /////////////
  // 和tcp一样
  
  while(1) {
      clnt_adr_sz = sizeof(clnt_adr);
      str_len = recvfrom(serv_sock, message, BUF_SIZE, 0,
                         (sockaddr*)& clnt_adr, &clnt_adr_sz);
      sendto(serv_sock, message, str_len, 0,
             (sockaddr*)& clnt_adr, clnt_adr_sz);
  }
  
  // 实际不会执行到这里
      close(serv_sock);
  ```

  
  
  
  
- 客户端:
  
  ```c++
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
  ```
  
  
  
  不同点:
  
  - 创建`socket`使用数据报套接字, 不同于tcp的`SOCK_STREAM`(流式套接字)

### UDP 客户端套接字地址分配

TCP使用`connect`自动完成把IP和端口分配给套接字的过程, 在UDP中调用`sendto`传输数据前分配套接字地址

, 所以UDP客户端无序额外的地址分配

### 数据边界问题

TCP不存在边界, 说明*数据传输过程中调用I/O函数的次数无意义*

而UDP具有数据边界, 传输中调用I/O很重要, 所以输入函数调用次数与输出函数调用次数**必须一致**

如: 客户端:

```c++
 for(int i = 0; i < 4; i++){
    sleep(5);
    adr_sz = sizeof(your_adr);
    str_len = recvfrom(sock, message, BUF_SIZE, 0,
                (struct sockaddr*)& your_adr, &adr_sz);

    printf("message %d : %s \n", i + 1, message);
}
```

服务端:

```c++
/// 发送这三条信息
sendto(sock, message1, sizeof(message1), 0, 
        (struct sockaddr*)& your_adr, sizeof(your_adr));
sendto(sock, message2, sizeof(message2), 0, 
    (struct sockaddr*)& your_adr, sizeof(your_adr));
sendto(sock, message3, sizeof(message3), 0, 
    (struct sockaddr*)& your_adr, sizeof(your_adr));
```

如果设置循环次数为3, 则正好接受, 如果设置大于3, 则接受三个后阻塞, 如果设置小于3, 则丢失数据

UDP存在数据边界, 1个数据包即可成为一个完整数据, 成为数据报

还有一个特点, 就是如果在未启动客户端接受数据时, 开启服务端就会直接传递数据, 传完即结束, 根本不等待, 所以是不可靠传输

### 两种状态: 已连接和未连接

- 未连接: (默认)

  每次使用`sendto`来指定目标地址, 发送过程涉及三个阶段:

  - 向套接字注册目标IP和端口
  - 传输数据
  - 删除注册的目标地址信息

- 已连接

  可以通过`connect`预先注册目标地址, 这样之后就可以使用`send`发送数据, 避免了每次发送都要重复注册/注销目标地址的开销(约占整个通信时间的1/3), 所以使用`connect`适用于与同一个主机长时间通信
