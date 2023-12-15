#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
const int BUFFER_SIZE = 1024;

int main() {
    // 创建套接字
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // 定义服务器地址结构
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12339); // 服务器端口
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器IP地址

    // 建立连接
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // 循环发送数据
    const char* message = "Hello from client!\n";
    int bytesSent = 0;
    int totalBytesSent = 0;

    while (totalBytesSent < strlen(message)) {
        bytesSent = send(clientSocket, message + totalBytesSent, strlen(message) - totalBytesSent, 0);

        if (bytesSent == -1) {
            std::cerr << "Error in send" << std::endl;
            break;
        }

        totalBytesSent += bytesSent;
    }

    // 循环接收数据
    char buffer[BUFFER_SIZE];
    int bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        // 处理接收到的数据
        std::cout << "Received: " << std::string(buffer, bytesRead) << std::endl;
    }

    // 关闭连接
    close(clientSocket);

    return 0;
}
