#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <fstream>
#include <direct.h>

using namespace std;

#define DIR_NAME "informe"
#define FILE_PATH "informe/puntaje.record"

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
        "ESTADISTICAS DEL JUEGO",
        "SALIR DEL JUEGO"
    };
    int seleccionado = 0;
    char tecla = 0;

    while (true) {
        clearScreen();
        mostrarEncabezado();
        cout << "Usa las flechas (arriba/abajo) y ENTER para seleccionar:\n\n";
        for (int i = 0; i < (int)opciones.size(); ++i) {
            if (i == seleccionado)
                cout << "  \x1B[7m> " << opciones[i] << " <\x1B[0m" << endl;
            else
                cout << "    " << opciones[i] << endl;
        }

        tecla = _getch();
        if (tecla == -32 || tecla == 224) {
            tecla = _getch();
            if (tecla == 72)
                seleccionado = (seleccionado - 1 + (int)opciones.size()) % (int)opciones.size();
            else if (tecla == 80)
                seleccionado = (seleccionado + 1) % (int)opciones.size();
        } else if (tecla == 13) {
            return seleccionado + 1;
        }
    }
}

void mostrarEstadisticas() {
    ifstream in(FILE_PATH);
    if (in.is_open()) {
        string label1, label2;
        int intentos = 0, mejorPuntaje = 0;

        in >> label1 >> intentos;
        in >> label2 >> mejorPuntaje;

        clearScreen();
        mostrarEncabezado();
        cout << "\n====== ESTADISTICAS DEL JUEGO ======\n";
        cout << "Intentos: " << intentos << endl;
        cout << "Mejor puntaje: " << mejorPuntaje << endl;
        cout << "====================================\n";

        in.close();
    } else {
        cout << "\nAun no hay estadisticas guardadas.\n";
    }

    cout << "\nPresiona cualquier tecla para continuar...\n";
    _getch();
}

#endif
