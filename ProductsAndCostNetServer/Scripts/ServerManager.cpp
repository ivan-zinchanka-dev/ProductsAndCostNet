#include "ServerManager.h"
#include "ProductQuery.h"

#include <sstream>
#include <list>
#include <vector>

volatile int ServerManager::_clientsCount = 0;
volatile ServerState ServerManager::_state = ServerState::WORKING;
ProductStore ServerManager::_productStore = ProductStore("Resources\\database.bin");

DWORD ServerManager::AdminCall(LPVOID lpVoid)
{
    try
    {
        //_productStore = ProductStore("Resources\\database.bin");
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
        
        cout << "Received: " << command <<endl;
        
        if (strcmp(command, VERIFY_PRODUCT) == 0)
        {
            cout << "Verify" <<endl;

            string productName = message.substr(COMMAND_SIZE, message.size());
            strcpy_s(messageBuffer, _productStore.ContainsProduct(productName) ? STR_TRUE : STR_FALSE);
            send(clientSocket, messageBuffer, sizeof(messageBuffer), 0);	
        }
        else if (strcmp(command, CALCULATE_QUERY_COST) == 0)
        {
            cout << "Calc" <<endl;
            //string serializedQuery = message.substr(COMMAND_SIZE, message.size());

            stringstream serializedQuery;
            serializedQuery.str(message.substr(COMMAND_SIZE, message.size()));
            
            string queryWordBuffer;
            vector<string> queryNames;
            vector<string> queryCounts;

            bool toNames = true;
            
            while (getline(serializedQuery, queryWordBuffer, QUERY_WORDS_DELIMITER))
            {
                toNames ? queryNames.emplace_back(queryWordBuffer) : queryCounts.emplace_back(queryWordBuffer);
                toNames = !toNames;
            }

            list<Product> productQuery;
            
            for (int i = 0; i < queryNames.size(); i++)
            {
                Product deserializedProduct = Product(queryNames.at(i).c_str(), Default, stoi(queryCounts.at(i)));
                productQuery.emplace_back(deserializedProduct);
            }

            for (auto p : productQuery)
            {
                cout << p << endl;
            }

            
            
            /*char* next_token = nullptr;
            char *word = strtok_s(const_cast<char*>(serializedQuery.c_str()), QUERY_WORDS_DELIMITER, &next_token); 

            while (word != nullptr) {
                
                word = strtok(nullptr, QUERY_WORDS_DELIMITER);
            }*/
            
            
        }
        else if (strcmp(command, COMPLETE_SESSION) == 0)
        {
            cout << "Complete" <<endl;
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
