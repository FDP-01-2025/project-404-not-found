#include <iostream>
#include <windows.h>
#include "./src/menu.h"
#include "./src/juego.h"
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif


using namespace std;

int main() {
    enableANSI();

    while (true) {
        int opcion = menuInteractivo(); 

        clearScreen();
        mostrarEncabezado();

        if (opcion == 1) {
            startGame(); 
        } else if (opcion == 2) {
            mostrarEstadisticas(); 
        } else {
            break; 
        }

        _getch();
    }

    return 0;
}
