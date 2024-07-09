#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ohos_init.h"
#include "cmsis_os2.h"
#include "lwip/sockets.h"
#include "wifi_connect.h"

#define WIFI_ACCOUNT "nova"
#define WIFI_PASSWD "13515072007"
#define PORT 8888

int sock_fd; // 修改为单个变量，因为我们只需要一个socket

char recvBuff[512];
const char *response = "'1'"; // 常量字符串

static void UDPServerTask(void) {
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    ssize_t ret;

    // 初始化WiFi连接
    WifiConnect(WIFI_ACCOUNT, WIFI_PASSWD);

    // 创建UDP socket
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket creation failed");
        exit(1);
    }

    // 设置服务器地址信息
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // 绑定socket
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind failed");
        exit(1);
    }

    printf("UDP server is running, waiting for messages...\n");

    while (1) { // 主循环处理接收和发送
        // 清理接收缓冲区
        memset(recvBuff, 0, sizeof(recvBuff));

        // 接收数据
        ret = recvfrom(sock_fd, recvBuff, sizeof(recvBuff), 0, (struct sockaddr *)&client_addr, &client_len);
        if (ret == -1) {
            perror("recvfrom failed");
            continue; // 如果接收失败，跳过本次循环
        }

        // 打印客户端信息和接收到的消息
        printf("Received message from %s:%d: %s\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), recvBuff);

        // 发送响应
        sendto(sock_fd, response, strlen(response) + 1, 0, (struct sockaddr *)&client_addr, client_len);
    }

    // 关闭socket（实际中，这里可能不会执行，因为while(1)是无限循环）
    close(sock_fd);
}

static void UDPServerEntry(void) {
    osThreadAttr_t threadAttr;
    memset(&threadAttr, 0, sizeof(threadAttr)); // 初始化线程属性
    threadAttr.stack_size = 10240;
    threadAttr.priority = osPriorityNormal; // 设置合适的线程优先级
    threadAttr.name = "UDPServerTask";

    if (osThreadNew((osThreadFunc_t)UDPServerTask, NULL, &threadAttr) == NULL) {
        printf("Failed to create UDPServerTask.\n");
    }
}

APP_FEATURE_INIT(UDPServerEntry);