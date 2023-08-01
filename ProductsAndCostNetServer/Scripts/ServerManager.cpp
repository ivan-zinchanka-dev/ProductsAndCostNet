#include "ServerManager.h"

volatile int ServerManager::_clientsCount = 0;
volatile ServerState ServerManager::_state = ServerState::WORKING;
ProductStore ServerManager::_productStore = ProductStore("Resources\\database.bin");

DWORD ServerManager::AdminCall(LPVOID lpVoid)
{
    try
    {
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
                _productStore.PrintAllProducts();
                break;
        
            case '2':
                _productStore.CreateProduct();
                break;

            case '3':
                _productStore.EditProduct();
                break;
                
            case '4':
                _productStore.TryRemoveProduct();
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

        string message(messageBuffer);
        string rawCommand = message.substr(0, COMMAND_SIZE);
        const char* command = rawCommand.c_str();
        
        if (strcmp(command, VERIFY_PRODUCT) == 0)
        {
            string productName = message.substr(COMMAND_SIZE, message.size());
            strcpy_s(messageBuffer, _productStore.ContainsProduct(productName) ? STR_TRUE : STR_FALSE);
            send(clientSocket, messageBuffer, sizeof(messageBuffer), 0);	
        }
        else if (strcmp(command, CALCULATE_QUERY_COST) == 0)
        {
            stringstream serializedQuery;
            serializedQuery.str(message.substr(COMMAND_SIZE, message.size()));
            
            ProductQuery *productQuery = new ProductQuery(serializedQuery);
            productQuery->UpdateProductsInfo(_productStore);

            Money cost = productQuery->CalculateCost();

            stringstream messageBufferStream;
            messageBufferStream << cost;
            strcpy_s(messageBuffer, messageBufferStream.str().c_str());
            send(clientSocket, messageBuffer, sizeof(messageBuffer), 0);	
            
            delete productQuery;
        }
        else if (strcmp(command, COMPLETE_SESSION) == 0)
        {
            break;
        }
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
