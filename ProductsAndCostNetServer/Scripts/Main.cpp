#include <winsock2.h>
#include <iostream>
#include <conio.h>

#pragma comment(lib, "Ws2_32.lib")

#include "ProductStore.h"
#include "ServerManager.h"

using namespace std;
using namespace ProductsLogic;

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

		if (listen(serverSocket, MAX_CLIENTS) != 0) {

			puts("Server listening error.\n");
			WSACleanup();
			return EXIT_FAILURE;
		}
		else {

			puts("Server initialisation complete.");	

			DWORD adminThreadId;
			CreateThread(nullptr, NULL, ServerManager::AdminCall, nullptr, NULL, &adminThreadId);
			
			sockaddr_in remoteAddress;
			int remoteAddressSize = sizeof(remoteAddress);
			
			SOCKET clientSocket;

			while (clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&remoteAddress), &remoteAddressSize)) {
				
				DWORD clientThreadId;
				CreateThread(nullptr, NULL, ServerManager::ClientCall, &clientSocket, NULL, &clientThreadId);
			}

			closesocket(clientSocket);
			
			WSACleanup();
			return EXIT_FAILURE;
		}
	}
}
