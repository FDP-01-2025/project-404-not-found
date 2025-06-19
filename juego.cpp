#include <iostream>
#include <conio.h>
#include <algorithm>
#include "funciones_juego.h"

using namespace std;

char pantalla[HEIGHT][WIDTH];
int jugadorX = WIDTH / 2;

vector<Shot> shots;
vector<int> ciempiesX;
int ciempiesY = 0;
int direccion = 1;
int cuentaInicial = 12;
const int separacion = 3;

int puntuacion = 0;
int vidas = 3;

int velocidadCiempies = 80;
clock_t ultimaMovidaCiempies = 0;
clock_t tiempoInicio;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO cursorInfo;

void iniciarCiempies() {
    ciempiesY = 0;
    ciempiesX.clear();
    for (int i = 0; i < cuentaInicial; i++) {
        ciempiesX.push_back((i * separacion) % WIDTH);
    }
}

void limpiarPantallaJuego() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            pantalla[y][x] = ' ';
        }
    }
}

void dibujarPantallaJuego() {
    pantalla[HEIGHT - 1][jugadorX] = '^';

    for (auto& shot : shots) {
        if (shot.active && shot.y >= 0 && shot.y < HEIGHT) {
            pantalla[shot.y][shot.x] = '|';
        }
    }

    for (int cx : ciempiesX) {
        if (ciempiesY < HEIGHT) {
            pantalla[ciempiesY][cx] = 'O';
        }
    }
}

void mostrarJuego() {
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);

    cout << "PUNTAJE: " << puntuacion << "   VIDAS: " << vidas << endl;
    cout << "+";
    for (int i = 0; i < WIDTH; i++) cout << "-";
    cout << "+" << endl;

    for (int y = 0; y < HEIGHT; y++) {
        cout << "|";
        for (int x = 0; x < WIDTH; x++) {
            cout << pantalla[y][x];
        }
        cout << "|" << endl;
    }

    cout << "+";
    for (int i = 0; i < WIDTH; i++) cout << "-";
    cout << "+" << endl;
}

void actualizarDisparos() {
    for (int i = 0; i < shots.size(); i++) {
        if (shots[i].active) {
            shots[i].y--;
            if (shots[i].y < 0) {
                shots[i].active = false;
            }
        }
    }

    for (int i = 0; i < shots.size(); i++) {
        if (!shots[i].active) continue;
        for (int j = 0; j < ciempiesX.size(); j++) {
            if (ciempiesY == shots[i].y && ciempiesX[j] == shots[i].x) {
                ciempiesX.erase(ciempiesX.begin() + j);
                shots[i].active = false;
                puntuacion += 10;
                break;
            }
        }
    }

    for (int i = 0; i < shots.size(); i++) {
        if (!shots[i].active) {
            shots.erase(shots.begin() + i);
            i--;
        }
    }
}

void actualizarCiempies() {
    bool chocoBorde = false;
    for (int cx : ciempiesX) {
        int nuevoX = cx + direccion;
        if (nuevoX < 0 || nuevoX >= WIDTH) {
            chocoBorde = true;
            break;
        }
    }

    if (chocoBorde) {
        direccion = -direccion;
        ciempiesY++;
    } else {
        for (int& cx : ciempiesX) {
            cx += direccion;
        }
    }

    if (ciempiesY >= HEIGHT - 1) {
        vidas--;
        iniciarCiempies();
    }

    if (ciempiesX.empty()) {
        cuentaInicial += 2;
        if (cuentaInicial > WIDTH / 2) cuentaInicial = WIDTH / 2;

        velocidadCiempies -= 5;
        if (velocidadCiempies < 20) velocidadCiempies = 20;

        iniciarCiempies();
    }
}

void actualizarJuego() {
    if (_kbhit()) {
        int tecla = _getch();
        if (tecla == 224) {
            int flecha = _getch();
            if (flecha == 75 && jugadorX > 0) jugadorX--;
            if (flecha == 77 && jugadorX < WIDTH - 1) jugadorX++;
        } else if (tecla == ' ') {
            if (shots.size() < 3) {
                Shot nuevoDisparo;
                nuevoDisparo.x = jugadorX;
                nuevoDisparo.y = HEIGHT - 2;
                nuevoDisparo.active = true;
                shots.push_back(nuevoDisparo);
            }
        }
    }

    actualizarDisparos();

    if (clock() - ultimaMovidaCiempies > velocidadCiempies * CLOCKS_PER_SEC / 1000) {
        actualizarCiempies();
        ultimaMovidaCiempies = clock();
    }
}

bool juegoTerminado() {
    return vidas <= 0;
}

void iniciarJuego() {
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    tiempoInicio = clock();
    jugadorX = WIDTH / 2;
    puntuacion = 0;
    vidas = 3;
    shots.clear();
    iniciarCiempies();

    system("cls");

    while (!juegoTerminado()) {
        limpiarPantallaJuego();
        dibujarPantallaJuego();
        mostrarJuego();
        actualizarJuego();
        Sleep(10);
    }

    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    system("cls");
    cout << "\n=== JUEGO TERMINADO ===\n";
    cout << "Puntaje final: " << puntuacion << endl;
    system("pause");
}
