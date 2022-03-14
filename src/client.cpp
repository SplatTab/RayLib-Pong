#include <iostream>
#include <winsock2.h>
#include <network.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996) 

#define PORT 7474

int client_socket;
sockaddr_in server;

void InitConnect(char* SERVER)
{
    WSADATA ws;
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0)
    {
        printf("Failed. Error Code: %d", WSAGetLastError());
    }
    printf("Initialised.\n");

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) // <<< UDP socket
    {
        printf("socket() failed with error code: %d", WSAGetLastError());
    }

    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER);
}

string NetworkSend(char message[BUFLEN]) {
        if (sendto(client_socket, message, strlen(message), 0, (sockaddr*)&server, sizeof(sockaddr_in)) == SOCKET_ERROR) {
            return "-0";
        }

        char answer[BUFLEN] = {};

        int slen = sizeof(sockaddr_in);
        int answer_length;
        if (answer_length = recvfrom(client_socket, answer, BUFLEN, 0, (sockaddr*)&server, &slen) == SOCKET_ERROR) {
            return "-1";
        }

        return answer;
}

void CloseConnect() {
    closesocket(client_socket);
    WSACleanup();
}