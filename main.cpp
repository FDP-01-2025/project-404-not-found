#include <iostream>
#include "./src/menu.h"
#include "./src/juego.h"

using namespace std;

int main() {
    enableANSI(); // Activa colores y caracteres especiales en consola (solo en Windows)

    while (true) {
        int opcion = menuInteractivo(); // Muestra el menú principal y devuelve la opción elegida

        clearScreen(); 
        mostrarEncabezado(); 

        if (opcion == 1) {
            startGame(); /*Inicia el juego/llama a la función que maneja el juego
                         en juego.h*/
        } else if (opcion == 2) {
            mostrarEstadisticas(); 
        } else {
            break; 
        }

        // Espera una tecla antes de volver al menú (compatible con Windows y Linux)
#ifdef _WIN32
        _getch();
#else
        cin.ignore();
        cin.get();
#endif
    }

    return 0; 
}
