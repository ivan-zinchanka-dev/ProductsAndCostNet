#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

#define MAXCON 5
#define MESSAGE_BUFFER_SIZE 500
#define DISCONNECT "@exit"  
#define VIEW "@v"

using namespace std;

volatile int clientsCount = 0;

char chat[MESSAGE_BUFFER_SIZE];

/*int Concat(char* destination, char* source) {

	int dest_size = strlen(destination);
	int result_size = dest_size + strlen(source);
	
	if (result_size + 2 > MESSAGE_BUFFER_SIZE) {
	
		return -1;	
	}	
	
	int j = 0;

	for (int i = dest_size; i < result_size; i++) {
	
		destination[i] = source[j];
		j++;		
	}

	destination[result_size] = '\n';
	destination[result_size + 1] = '\0';

	return 0;
}*/



DWORD WINAPI ClientCall(LPVOID clientSocketPtr) {

	SOCKET clientSocket = static_cast<SOCKET*>(clientSocketPtr)[0];

	char messageBuffer[MESSAGE_BUFFER_SIZE];

	strcpy_s(messageBuffer, "CONNECTED_TO_SERVER");

	send(clientSocket, messageBuffer, sizeof(messageBuffer), 0);

	while (recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0)) {
		
		/*if (strcmp(messageBuffer, DISCONNECT) == 0) {
			
			break;	
		}
		else if ((strcmp(messageBuffer, VIEW) == 0) || (Concat(chat, messageBuffer) == 0)) {
		
			strcpy_s(messageBuffer, chat);		
		}

		else {
			
			strcpy_s(messageBuffer, "Chat Overflow!");	
		}*/	

		send(clientSocket, messageBuffer, sizeof(messageBuffer), 0);	
	} 

	clientsCount--;

	closesocket(clientSocket);

	return EXIT_SUCCESS;
}


int main(int argc, char* argv[]) {

	WORD wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {

		puts("Server initialization error.\n");
		return EXIT_FAILURE;
	}
	else
	{
		sockaddr_in localAddress;
		localAddress.sin_family = AF_INET;
		localAddress.sin_port = htons(1280);
		localAddress.sin_addr.s_addr = htonl(INADDR_ANY);

		SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

		if (bind(serverSocket, reinterpret_cast<sockaddr*>(&localAddress), sizeof(localAddress)) != 0) {

			puts("Server initialization error.\n");
			WSACleanup();
			return EXIT_FAILURE;
		}

		if (listen(serverSocket, MAXCON) != 0) {

			puts("Server listening error.\n");
			WSACleanup();
			return EXIT_FAILURE;
		}
		else {

			puts("Server initialisation complete.");	

			sockaddr_in remoteAddress;
			int remoteAddressSize = sizeof(remoteAddress);
		
			SOCKET clientSocket;

			while (clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&remoteAddress), &remoteAddressSize)) {
		
				clientsCount++;

				if (clientsCount > 0) {
			
					printf_s("Clients online: %d.\n", clientsCount);	
				}
				else {
			
					puts("No clients in online.");					
				}
		
				DWORD threadId;
				CreateThread(NULL, NULL, ClientCall, &clientSocket, NULL, &threadId);		
			}

			closesocket(clientSocket);

			WSACleanup();
			return EXIT_FAILURE;
		}
		
	}
	
}
