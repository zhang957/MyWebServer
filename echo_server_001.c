#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include<unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc,char* argv[])
{
    //检查命令行参数的数量，确保提供了IP地址和端口号。
    if(argc <= 2)
    {
        printf("Usage: %s ip_address portname\n", argv[0] );
        return 0;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd >=1);
    
    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;//地址族
    address.sin_port = htons(port);//将主机序列转化为网络序，因为大小端的不同
    //将点分十进制的ip地址转化为用于网络传输的数值格式
    //返回值：若成功则为1，若输入不是有效的表达式则为0，若出错则为-1
    inet_pton(AF_INET,ip,&address.sin_addr);
    //这段代码的目的是将套接字描述符 listenfd 与特定的地址结构体 address 绑定在一起，如果绑定失败，则通过 assert 终止程序
    int ret = 0;
    ret = bind(listenfd,(struct sockaddr*)(&address),sizeof(address));
    assert(ret != -1);
    //将服务器套接字 listenfd 标记为被动套接字，并设置连接请求队列的最大长度为 5
    ret = listen(listenfd,5);
    assert(ret != -1);
    //通过 accept 函数接受客户端连接请求，获取客户端的地址信息，并创建一个新的套接字 sockfd 用于与客户端进行通信
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    //accept函数是一个阻塞函数，只有等到有链接请求，返回并创建一个套接字与客户端通信
    int sockfd = accept(listenfd,(struct sockaddr*)(&client),&client_addrlength);
    char buf_size[1024] = {0};
    int recv_size = 0;
    //从套接字sockfd接收数据，并将数据存储在buf_size中
    recv_size = recv(sockfd,buf_size,recv_size,0);
    //使用 send 函数向已连接的套接字发送数据，发送的数据来自 buf_size，发送的字节数为 recv_size
    int send_size = 0;
    send_size = send(sockfd,buf_size,recv_size,0);

    close(sockfd);
    close(listenfd);
    return 0;
}