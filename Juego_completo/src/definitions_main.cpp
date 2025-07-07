#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <string>
#include "menu.h"

using namespace std;

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
    cout << "\x1B[2J\x1B[H";
}

void mostrarEncabezado() {
    cout << R"(
=====================================================
  ____ _____ _   _ _____ ___ ____  _____ ____  _____ 
 / ___| ____| \ | |_   _|_ _|  _ \| ____|  _ \| ____|
| |   |  _| |  \| | | |  | || |_) |  _| | | | | |_   
| |___| |___| |\  | | |  | ||  __/| |___| |_| | |___ 
 \____|_____|_| \_| |_| |___|_|   |_____|____/|_____|

=====================================================
)" << "\n";
}

int menuInteractivo() {
    vector<string> opciones = {
        "INICIAR NUEVA PARTIDA",
        "VER ESTADISTICAS DEL JUEGO",
        "SALIR DEL JUEGO"
    };
    int seleccionado = 0;
    char tecla = 0;

    while (true) {
        clearScreen();
        mostrarEncabezado();
        cout << "Usa las flechas (arriba/abajo) y ENTER para seleccionar:\n\n";
        for (int i = 0; i < opciones.size(); ++i) {
            if (i == seleccionado)
                cout << "  \x1B[7m> " << opciones[i] << " <\x1B[0m" << endl;
            else
                cout << "    " << opciones[i] << endl;
        }
        tecla = _getch();
        if (tecla == -32 || tecla == 224) {
            tecla = _getch();
            if (tecla == 72)
                seleccionado = (seleccionado - 1 + opciones.size()) % opciones.size();
            else if (tecla == 80)
                seleccionado = (seleccionado + 1) % opciones.size();
        } else if (tecla == 13) {
            return seleccionado + 1;
        }
    }
}
