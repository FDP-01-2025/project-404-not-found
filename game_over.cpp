#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

void habilitarANSI() {
    DWORD mode = 0;
    GetConsoleMode(hStdout, &mode);
    SetConsoleMode(hStdout, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void limpiarPantalla() {
    cout << "\x1B[2J\x1B[H";  // o system("cls");
}
using namespace std;
void limpiar_pantalla()
{
    system("cls");
}

void mostrar_pantalla_gameover()
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

int pantalla_completa_gameover()
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
        limpiar_pantalla();
        mostrar_pantalla_gameover();
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
        if (tecla == -32 || tecla = 224)
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
    bool seguir_jugando = true;

    while (seguir_jugando)
    {
        limpiar_pantalla();
        cout <<"SIMULANDO PARTIDA ....\n";
        Sleep(2000); 

        int opcion = mostrar_pantalla_gameover();

        if (opcion == 1)
        {
            cout << "\nReiniciando partida...\n";
            Sleep(1000);
        }
        else
        {
            cout <<"\n Gracias por jugar. Hasta luego gamer :) !\n"
            seguir_jugando = false;
        }
    }
    return 0;
}
