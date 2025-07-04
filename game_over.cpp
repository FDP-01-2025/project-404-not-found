#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

void enableANSI() {
    DWORD mode = 0;
    GetConsoleMode(hStdout, &mode);
    SetConsoleMode(hStdout, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void clearScreen() {
    cout << "\x1B[2J\x1B[H";  // o system("cls");
}
using namespace std;
void clear_screen()
{
    system("cls");
}

void show_game_over_screen()
{
    cout << R"(
 ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ 
██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗
██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝
██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║██║   ██║██╔══╝  ██╔══██╗
╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝╚██████╔╝███████╗██║  ██║
 ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚══▀▀═╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝
                                                                        
    )" << "\n";
}

int complete_screen_gameover()
{
    vector<string> opciones =
    {
        "JUGAR DE NUEVO",
        "SALIR"
    };


    int seleccionado = 0;
    char tecla;

    while (true)
    {
        clear_screen();
        show_game_over_screen();
        cout <<"\n ¡HAS PERDIDO LA PARTIDA! \n";
        cout <<"\n Usa las flechas (arriba/abajo) y ENTER para seleccionar:\n\n";

        for (int i = 0; i < opciones.size(); i++)
        {
            if (i == seleccionado)
            {
                cout << "\t  > " << opciones[i] << " <" << "\n";
            }
            else
            {
                cout << "\t    " << opciones[i] << "\n";
            }
        }
        tecla = _getch();
        if (tecla == -32 || tecla == 224)
        {
            tecla = _getch();
            if (tecla == 72)
            {
            seleccionado = (seleccionado - 1 + opciones.size()) % opciones.size();
            }
            else if(tecla == 80)
            {
                seleccionado = (seleccionado + 1) % opciones.size();
            }
        }
        else if(tecla == 13)
        {
        return seleccionado +1;
        }
    }
} 
int main ()
{
    bool keep_playing = true;

    while (keep_playing)
    {
        clear_screen();
        cout <<"SIMULANDO PARTIDA ....\n";
        Sleep(2000); 

        int opcion = complete_screen_gameover();

        if (opcion == 1)
        {
            cout << "\nReiniciando partida...\n";
            Sleep(1000);
        }
        else
        {
            cout <<"\n Gracias por jugar. Hasta luego gamer :) !\n";
            keep_playing = false;
        }
    }
    return 0;
}
