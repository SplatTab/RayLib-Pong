#include <iostream>
#include <winsock2.h>
#include <string>
#include<vector>

using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 

#define BUFLEN 512
#define PORT 7474

int cons;

typedef struct pos {
    int x;
    int y;
} pos;

vector<pos> positions;

int main()
{
    system("title UDP Server");

    sockaddr_in server, client;
    WSADATA wsa;
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code: %d", WSAGetLastError());
        exit(0);
    }
    printf("Server Started.\n");

    SOCKET server_socket;
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket: %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done.");

    while (true)
    {
        printf("Waiting for data...");
        fflush(stdout);
        char message[BUFLEN] = {};

        int message_len;
        int slen = sizeof(sockaddr_in);
        if (message_len = recvfrom(server_socket, message, BUFLEN, 0, (sockaddr*)&client, &slen) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code: %d", WSAGetLastError());
            exit(0);
        }

        printf("Received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("Data: %s\n", message);

        //Tell server you are connecting
        if (message[0] == 'C') {
            cons++;
            pos p = {0, 0};
            positions.push_back(p);
            sprintf(message, "%d", cons);
            //Get connections number
        } else if (message[0] == 'G' && message[1] == 'C') {
            sprintf(message, "%d", cons);
            //Update position
        } else if (message[0] == 'P')  {
            string data(message);
            string nop1 = data.substr(3);
            size_t ypos = nop1.find("Y");

            string x = nop1.substr(0, ypos);
            string y = nop1.substr(ypos + 1);
            
            positions.at(stoi(data.substr(1, 1))).x = stoi(x);
            positions.at(stoi(data.substr(1, 1))).y = stoi(y);
            //Get a plyers positions
        } else if (message[0] == 'G' && message[1] == 'P') {
            string data(message);
            sprintf(message, "%dY%d", positions.at(stoi(data.substr(2, 2))).x, positions.at(stoi(data.substr(2, 2))).y);
        }

        if (sendto(server_socket, message, strlen(message), 0, (sockaddr*)&client, sizeof(sockaddr_in)) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code: %d", WSAGetLastError());
            return 3;
        }
    }

    closesocket(server_socket);
    WSACleanup();
}