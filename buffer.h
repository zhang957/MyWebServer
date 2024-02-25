#ifndef BUFFER_H
#define BUFFER_H
#include <cstring>   // perror
#include <iostream>
#include <unistd.h>  // write
#include <sys/uio.h> // readv
#include <vector>     // readv
#include <atomic>
#include <assert.h>
class Buffer
{
private:
    /* data */
    //返回缓冲区的开始指针
    const char* BeginPtr_() const;
    char* BeginPtr_();
    //确保缓冲区有足够的空间容纳指定长度的数据
    void MakeSpace(size_t len);
    //使用std::vector<char>存储数据
    std::vector<char> buffer_;
    //读取位置和写入位置，用原子操作来保证线程安全
    std::atomic<std::size_t> readPos_;
    std::atomic<std::size_t> writePos_;

public:
    Buffer(/* args */int initBuffSize = 1024);
    ~Buffer() = default;
    //返回可写入字节数，可读取的字节书，前置字节数
    size_t WriteableBytes() const;
    size_t ReadableBytes() const;
    size_t PrependableBytes() const;//前置字节数
    //返回缓冲区读取位置的指针
    const char* peak() const;
    //确保缓冲有足够的写入空间
    void EnsureWriteable(size_t len);
    //标记已经写入的字节数
    void HasWriteable(size_t len);
    //从缓冲区读入指定长度的数据
    void Retrieve(size_t len);
    //从缓冲区中读到指定的结束字符
    void RetrieveUntil(const char* end);
    //重置缓冲区
    void RetrieveAll();
    //将缓冲区的数据变为字符串类型
    std::string RetrieveAllToStr();
    //返回缓冲区写入位置处的指针（分const和非const)
    const char* BeginWriteConst() const;
    char* BeginWrite();
    //向缓冲区追加数据（支持字符串，字符数组，二进制数）
    void Append(const std::string& str);
    void Append(const char* str,size_t len);
    void Append(const void* data,size_t lend);
    void Append(const Buffer& buf);
    //从文件描述符中读取数据到缓冲区，从缓冲区写入数据到文件描述符
    //记住读取是接收文件描述符的数据，写入是发送数据到文件描述符
    ssize_t ReadFd(int fd,int* Errno);
    ssize_t WriteFd(int fd,int* Errno);

};
#endif