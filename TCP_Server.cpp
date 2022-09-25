#include <stdio.h>
#include <tchar.h>
#include <WinSock2.h>
#include <SDKDDKVer.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
    system("title TCP_Server");

    //检查协议栈是否可使用
    WSADATA wsaData;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        printf("error:WSAStartup() failed\n");
        return -1;
    }

    printf("Server Online...\n");

    // Create Server Socket
    SOCKET SockServer = socket(AF_INET, SOCK_STREAM, 0);

    // Creaate Server Address
    SOCKADDR_IN addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrServer.sin_port = htons(2000);

    // bind socket
    if (SOCKET_ERROR == bind(SockServer, (SOCKADDR *)&addrServer, sizeof(SOCKADDR)))
    {
        printf("error:bind() failed...%d\n", WSAGetLastError());
        return -1;
    }

    // Listeen
    if (SOCKET_ERROR == listen(SockServer, 5))
    {
        printf("error: listen() failed...%d\n", WSAGetLastError());
        return -1;
    }

    printf("Listening...\n");

    // Init Client Address Parameter
    int len = sizeof(SOCKADDR);
    SOCKADDR_IN addrClient;
    char szSendBuf[2048] = "Connect Server Successfully";
    char szRecvBuf[2048] = "";

    // Waiting User Connection
    SOCKET sockAccept = accept(SockServer, (SOCKADDR *)&addrClient, &len);
    if (INVALID_SOCKET == sockAccept)
    {
        printf("error:  accept() faile...:%d\n", WSAGetLastError());
        return -1;
    }

    // Output Message
    printf("Accepted client IP:[%s],prot:[%d]\n", inet_ntoa(addrClient.sin_addr), ntohs(addrClient.sin_port));

    // Loop
    while (true)
    {
        recv(sockAccept, szRecvBuf, sizeof(szRecvBuf), 0);
        printf("Client:%s \n", szRecvBuf);
    }

    // Close Socket
    closesocket(SockServer);
    WSACleanup();
    return 0;
}