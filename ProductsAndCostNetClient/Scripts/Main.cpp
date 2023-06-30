#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <iostream>
#include <conio.h>
//#include <windows.h>
#include <algorithm>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define MESSAGE_BUFFER_SIZE 500
#define PING "@ping"
#define DISCONNECT "@exit"
#define VIEW "@v"
#define NAME "Ivan"


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    HDC hDC; // создаём дескриптор ориентации текста на экране
    PAINTSTRUCT ps; // структура, сод-щая информацию о клиентской области (размеры, цвет и тп)
    RECT rect; // стр-ра, определяющая размер клиентской области
    COLORREF colorText = RGB(255, 0, 0); // задаём цвет текста
    switch(uMsg){
    case WM_PAINT: // если нужно нарисовать, то:
        hDC = BeginPaint(hWnd, &ps); // инициализируем контекст устройства
        GetClientRect(hWnd, &rect); // получаем ширину и высоту области для рисования
        SetTextColor(hDC, colorText); // устанавливаем цвет контекстного устройства
        DrawText(hDC, L"Text", -1, &rect, DT_SINGLELINE|DT_CENTER|DT_VCENTER); // рисуем текст
        EndPaint(hWnd, &ps); // заканчиваем рисовать
        break;
    case WM_DESTROY: // если окошко закрылось, то:
        PostQuitMessage(NULL); // отправляем WinMain() сообщение WM_QUIT
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam); // если закрыли окошко
    }
    return NULL; // возвращаем значение
}

// Основная функция - аналог int main() в консольном приложении:
int WINAPI WinMain(HINSTANCE hInstance, // дескриптор экземпляра приложения
                   HINSTANCE hPrevInstance, // в Win32 не используется
                   LPSTR lpCmdLine, // нужен для запуска окна в режиме командной строки
                   int nCmdShow) // режим отображения окна
{

    HWND mainWindowDesc;
    WNDCLASSEX mainWindow;
    mainWindow.cbSize = sizeof(mainWindow); // размер структуры (в байтах)
    //mainWindow.lpfnWndProc = WndProc;

    
    // Функция вывода окна с кнопкой "ОК" на экран (о параметрах позже)
    MessageBox(NULL, L"HELLO!!!", L"Window", MB_OK);
    return NULL; // возвращаем значение функции
}






