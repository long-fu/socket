#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <algorithm>
#include <arpa/inet.h> // 包含 inet_addr 函数的头文件
#include <unistd.h>    // 包含 close 函数的头文件

const int BUFFER_SIZE = 1024;

void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    // 用于存储完整的消息
    std::string completeMessage;

    printf("处理客户端消息\n");
    // 循环接收数据
    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        // 将接收到的数据追加到完整的消息
        completeMessage += std::string(buffer, bytesRead);

        // 检查消息是否已接收完整 特定的数据 来判断消息返程
        size_t pos = completeMessage.rfind('\n');
        if (pos != std::string::npos) {
            // 处理完整的消息
            std::cout << "Received complete message: " << completeMessage.substr(0, pos) << std::endl;

            // 处理业务逻辑，这里简单回显
            completeMessage += " 我是服务器回！！！";
            send(clientSocket, completeMessage.c_str(), completeMessage.length(), 0);

            // 清空完整的消息，准备接收下一个消息
            completeMessage.clear();
        }
    }

    // 客户端关闭连接或发生错误
    if (bytesRead == 0) {
        std::cout << "Client disconnected" << std::endl;
    } else {
        std::cerr << "Error in recv" << std::endl;
    }

    // 关闭连接
    close(clientSocket);
}

int main() {
    // 创建套接字
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // 定义服务器地址结构
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12348); // 选择一个端口
    serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0"); // 服务器IP地址

    // 绑定套接字
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        return -1;
    }

    // 监听连接请求
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        close(serverSocket);
        return -1;
    }

    std::vector<std::thread> threads;

    // 循环接受连接
    while (true) {
        // 接受连接
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        if (clientSocket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        // 创建新线程处理客户端连接
        threads.emplace_back(handleClient, clientSocket);

        // 清理已经完成的线程
        threads.erase(std::remove_if(threads.begin(), threads.end(),
                        [](std::thread &t) { return !t.joinable(); }), threads.end());

        // std::thread{handleClient,clientSocket}.detach();

    }

    // 不会执行到这里，因为服务器一般会一直运行

    // 关闭服务器套接字
    close(serverSocket);

    return 0;
}
