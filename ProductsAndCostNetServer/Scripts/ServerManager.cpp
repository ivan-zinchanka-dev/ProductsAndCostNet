#include "ServerManager.h"

volatile int ServerManager::_clientsCount = 0;
volatile ServerState ServerManager::_state = ServerState::WORKING;

DWORD ServerManager::AdminCall(LPVOID lpVoid)
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

        ShutDownDemand();
    }
    catch (exception &ex)
    {
        cout << ex.what() << endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

DWORD ServerManager::ClientCall(LPVOID clientSocketPtr)
{
    _clientsCount++;
    
    SOCKET clientSocket = static_cast<SOCKET*>(clientSocketPtr)[0];

    char messageBuffer[MESSAGE_BUFFER_SIZE];
    strcpy_s(messageBuffer, PING);
    send(clientSocket, messageBuffer, sizeof(messageBuffer), 0);

    while (recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0)) {
        
        send(clientSocket, messageBuffer, sizeof(messageBuffer), 0);	
    } 

    _clientsCount--;
    
    closesocket(clientSocket);

    return EXIT_SUCCESS;
}

int ServerManager::GetClientsCount()
{
    return _clientsCount;
}

ServerState ServerManager::GetState()
{
    return _state;
}

void ServerManager::ShutDownDemand()
{
    _state = ServerState::SHUTTING_DOWN;
}
