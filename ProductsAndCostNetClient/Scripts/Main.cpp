#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <conio.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define MESSAGE_BUFFER_SIZE 500
#define DISCONNECT "@exit"
#define VIEW "@v"
#define NAME "Ivan"


int main() {

    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    
    if (WSAStartup(wVersionRequested, &wsaData) != 0) {

        puts("Initialization error.\n");
        return EXIT_FAILURE;
    }
    else
    {
        sockaddr_in remoteAddress;
        remoteAddress.sin_family = AF_INET;
        remoteAddress.sin_port = htons(1280);
        remoteAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (connect(clientSocket, reinterpret_cast<sockaddr*>(&remoteAddress), sizeof(remoteAddress)) != 0) {

            puts("Connection error.");
            WSACleanup();
            return EXIT_FAILURE;
        }
        else {
            
            char messageBuffer[MESSAGE_BUFFER_SIZE];
            recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);
            puts(messageBuffer);
            
           
        
            bool needToExit = false;
        
            while (!needToExit)
            {
                cout << "1. View all products\n"
                    "2. Add product\n"
                    "3. Edit product\n"
                    "4. Remove product\n"
                    "0. Exit\n";


                char choice = static_cast<char>(_getch());
                
                strcpy_s(messageBuffer, reinterpret_cast<char*>(choice));
                send(clientSocket, messageBuffer, sizeof(messageBuffer), 0);
                
            }


            closesocket(clientSocket);
        }

        
    }

    

    WSACleanup();
    return 0;
}


int main(int argc, char* argv[])
{
    try
    {
        ProductStore productStore("Resources\\database.bin");
        bool needToExit = false;
        
        while (!needToExit)
        {
            cout << "1. View all products\n"
                "2. Add product\n"
                "3. Edit product\n"
                "4. Remove product\n"
                "0. Exit\n";
        
            switch (_getch())
            {

            case '1':
                productStore.PrintAllProducts();
                break;
        
            case '2':
                productStore.CreateProduct();
                break;

            case '3':
                productStore.EditProduct();
                break;
                
            case '4':
                productStore.TryRemoveProduct();
                break;

            case '0':
                needToExit = true;
                break;

            default:
                break;
            }
        }
        
    }
    catch (exception &ex)
    {
        cout << ex.what() << endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

