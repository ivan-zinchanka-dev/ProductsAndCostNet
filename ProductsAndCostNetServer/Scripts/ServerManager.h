#pragma once

#include "ProductStore.h"
#include "ProductQuery.h"

#include <windows.h>
#include <conio.h>
#include <sstream>

using namespace ProductsLogic;

#define MAX_CLIENTS 10
#define CONNECTION_ERROR_MSG "Connection error was occurred"

#define MESSAGE_BUFFER_SIZE 500
#define COMMAND_SIZE 20

#define PING "@ping_______________"
#define COMPLETE_SESSION "@complete_session___"
#define VERIFY_PRODUCT "@verify_product_____"
#define CALCULATE_QUERY_COST "@calc_query_cost____"

#define STR_TRUE "1"
#define STR_FALSE "0"

class ServerManager
{
private:
    
    static volatile int _clientsCount;
    static ProductStore _productStore;
    
public:
    
    static DWORD WINAPI AdminCall(LPVOID);
    static  DWORD WINAPI ClientCall(LPVOID);
    static int GetClientsCount();
};
