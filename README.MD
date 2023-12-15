# socket例子

服务端无法正确判断数据完全被接收！！


在一个基于流的套接字（如TCP）中，接收端通常无法准确判断一个消息何时结束。这是因为TCP是一个流协议，消息被视为一个连续的字节流，而没有消息边界的概念。因此，接收端需要通过其他方式来判断消息的结束。

以下是几种常见的方法：

消息长度前缀： 在消息前面附加一个表示消息长度的前缀，接收端首先接收前缀以获取消息长度，然后再接收指定长度的消息内容。这样接收端可以知道何时接收到了完整的消息。

```cpp
// 服务器端接收消息的示例
std::string receiveMessage(int socket) {
    char buffer[BUFFER_SIZE];
    std::string receivedMessage;

    while (true) {
        int bytesRead = recv(socket, buffer, sizeof(buffer), 0);

        if (bytesRead <= 0) {
            // 处理连接关闭或出错的情况
            break;
        }

        receivedMessage.append(buffer, bytesRead);

        // 寻找分隔符，例如换行符
        size_t delimiterPos = receivedMessage.find('\n');
        if (delimiterPos != std::string::npos) {
            break; // 找到分隔符，消息结束
        }
    }

    return receivedMessage;
}
```

在这个例子中，uint32_t 表示消息的长度，服务器先接收这个前缀，再根据前缀中指定的长度接收实际的消息内容。

特殊分隔符： 使用特殊字符或字符序列作为消息的分隔符，例如换行符或其他不会出现在消息内容中的字符。接收端通过寻找分隔符来确定消息何时结束。

```cpp

// 服务器端接收消息的示例
std::string receiveMessage(int socket) {
    uint32_t messageLength;
    recv(socket, &messageLength, sizeof(uint32_t), 0);
    messageLength = ntohl(messageLength);

    char buffer[messageLength + 1];
    recv(socket, buffer, messageLength, 0);
    buffer[messageLength] = '\0';

    return buffer;
}

```
在这个例子中，接收端通过寻找换行符 \n 来确定消息何时结束。

这两种方法都有各自的优缺点，具体选择取决于应用的需求。需要注意的是，在实际应用中，通常需要处理粘包和拆包的问题，这可能需要更复杂的协议设计或者使用现有的协议库。!!!

本例子 用的是用特殊字符来进行数据判断数据是否发送完全!!