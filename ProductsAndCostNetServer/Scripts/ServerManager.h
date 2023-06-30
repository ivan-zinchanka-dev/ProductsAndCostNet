#pragma once
#include <windows.h>
#include <conio.h>
#include "ProductStore.h"


using namespace ProductsLogic;

#define PING "@ping"
#define COMPLETE_SESSION "@complete_session"

#define MAX_CLIENTS 10
#define MESSAGE_BUFFER_SIZE 500

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
    
public:

    
    static DWORD WINAPI AdminCall(LPVOID);
    static  DWORD WINAPI ClientCall(LPVOID);
    static int GetClientsCount();
    static ServerState GetState();
    static void ShutDownDemand();
    
};
