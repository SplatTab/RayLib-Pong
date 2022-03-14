#define BUFLEN 512

#include <string>

void InitConnect(char* SERVER);
std::string NetworkSend(char message[BUFLEN]);
std::string ListenMessage();
void CloseConnect();