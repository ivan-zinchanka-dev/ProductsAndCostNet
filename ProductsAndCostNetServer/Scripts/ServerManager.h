#pragma once
#include <windows.h>
#include <conio.h>
#include "ProductStore.h"


using namespace ProductsLogic;

#define PING "@ping_______________"
#define COMPLETE_SESSION "@complete_session___"
#define VERIFY_PRODUCT "@verify_product_____"
#define CALCULATE_QUERY_COST "@calc_query_cost____"

#define COMMAND_SIZE 20
#define MESSAGE_BUFFER_SIZE 500
#define CONNECTION_ERROR_MSG "Connection error was occurred"
#define MAX_CLIENTS 10

#define STR_TRUE "1"
#define STR_FALSE "0"


enum class ServerState
{
    WORKING = 0,
    SHUTTING_DOWN = 1,
};

class ServerManager
{
private:
    
    static volatile int _clientsCount;
    static volatile ServerState _state;
    static ProductStore _productStore;
    
public:
    
    static DWORD WINAPI AdminCall(LPVOID);
    static  DWORD WINAPI ClientCall(LPVOID);
    static int GetClientsCount();
    static ServerState GetState();
    static void ShutDownDemand();
    
};
