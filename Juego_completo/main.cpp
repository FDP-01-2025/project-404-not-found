#include <iostream>
#include <conio.h>
#include <windows.h>
#include "./src/menu.h"

using namespace std;

int main() {
    enableANSI();
    while (true) {
        int opcion = menuInteractivo();

        clearScreen();
        mostrarEncabezado();
        if (opcion == 1) {
            cout << "\nCARGANDO PARTIDA...\n";
            Sleep(1000);
            startGame(); 
        } else if (opcion == 2) {
            clearScreen();
            mostrarEncabezado();
            mostrarEstadisticas(); 
        } else {
            cout << "\nGRACIAS POR JUGAR\n";
            break;
        }

        cout << "\nPresiona cualquier tecla para volver al menú principal...\n";
        _getch();
    }
    return 0;
}
