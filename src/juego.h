#ifndef JUEGO_H
#define JUEGO_H
// Librerías para Windows/Linux
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <string>
// Librerías para manejo de consola y archivos por sistema operativo que se este usando
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#include <direct.h>
#else
#include <unistd.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/stat.h>
#endif

using namespace std;

#define DIR_NAME "informe"
#define FILE_PATH "informe/puntaje.record"

// Constantes del tamaño del juego y disparos
const int WIDTH = 30;
const int HEIGHT = 20;
const int MAX_SHOTS = 3;

// Estructura para los disparos
struct Shot {
    int x, y;
    bool active;
};

// Variables globales del juego
char pantalla[HEIGHT][WIDTH];
int playerX = WIDTH / 2;
vector<Shot> shots;
vector<int> centipedeX;
int centipedeY = 0;
int direction = 1;
int initialCount = 12;
const int separation = 3;
int score = 0;
int lives = 3;
int centipedeSpeed = 80;
clock_t lastmoveCentipede = 0;
clock_t startTime;

#ifdef _WIN32
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO cursorInfo;

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

// Habilita colores ANSI en Windows
void enableANSI() {
    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

#else
void enableANSI() {}
#endif

// Limpia la pantalla para el game over
void clearScreenGameover() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Limpia la pantalla del juego
void clearScreenGame() {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            pantalla[y][x] = ' ';
}

// Dibuja los elementos del juego en la matriz pantalla
void drawScreenGame() {
    pantalla[HEIGHT - 1][playerX] = '^'; // Jugador
    for (const auto& shot : shots)
        if (shot.active && shot.y >= 0 && shot.y < HEIGHT)
            pantalla[shot.y][shot.x] = '|'; // Disparos

    for (int cx : centipedeX)
        if (centipedeY < HEIGHT)
            pantalla[centipedeY][cx] = 'O'; // Ciempiés
}

// Muestra la pantalla del juego en consola
void showGame() {
#ifdef _WIN32
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);
#else
    cout << "\033[H"; 
#endif

    cout << "PUNTAJE: " << score << "   VIDAS: " << lives << endl;
    cout << "+";
    for (int i = 0; i < WIDTH; i++) cout << "-";
    cout << "+" << endl;

    for (int y = 0; y < HEIGHT; y++) {
        cout << "|";
        for (int x = 0; x < WIDTH; x++)
            cout << pantalla[y][x];
        cout << "|" << endl;
    }

    cout << "+";
    for (int i = 0; i < WIDTH; i++) cout << "-";
    cout << "+" << endl;

    // Mostrar controles del juego debajo del tablero
    cout << "\nControles: ";
    cout << "[TECLAS IZQ/DER] Mover   ";
    cout << "[\033[1mESPACIO\033[0m] Disparar" << endl;
}

// Guarda estadísticas del jugador en un archivo
void guardarEstadisticas(int nuevoPuntaje) {
#ifdef _WIN32
    _mkdir(DIR_NAME);
#else
    mkdir(DIR_NAME, 0777);
#endif

    int intentos = 0;
    int mejorPuntaje = 0;

    ifstream in(FILE_PATH);
    if (in.is_open()) {
        string tmp;
        in >> tmp >> intentos;
        in >> tmp >> mejorPuntaje;
        in.close();
    }

    intentos++;
    if (nuevoPuntaje > mejorPuntaje) {
        mejorPuntaje = nuevoPuntaje;
    }

    ofstream out(FILE_PATH);
    out << "Intentos: " << intentos << endl;
    out << "MejorPuntaje: " << mejorPuntaje << endl;
    out.close();
}

// Inicializa el ciempiés al inicio o cuando se reinicia
void startCentipede() {
    centipedeY = 0;
    centipedeX.clear();
    for (int i = 0; i < initialCount; i++) {
        centipedeX.push_back((i * separation) % WIDTH);
    }
}

// Actualiza la posición de los disparos y verifica colisiones
void updateShots() {
    for (auto& shot : shots) {
        if (shot.active) {
            shot.y--;
            if (shot.y < 0)
                shot.active = false;
        }
    }

    for (auto& shot : shots) {
        if (!shot.active) continue;
        for (size_t j = 0; j < centipedeX.size(); ++j) {
            if (centipedeY == shot.y && centipedeX[j] == shot.x) {
                centipedeX.erase(centipedeX.begin() + j);
                shot.active = false;
                score += 10;
                break;
            }
        }
    }

    shots.erase(
        remove_if(shots.begin(), shots.end(), [](const Shot& s) { return !s.active; }),
        shots.end()
    );
}

// Mueve el ciempiés y verifica si toca los bordes o baja de nivel
void updateCentipede() {
    bool hitBorder = false;
    for (int cx : centipedeX) {
        int newX = cx + direction;
        if (newX < 0 || newX >= WIDTH) {
            hitBorder = true;
            break;
        }
    }

    if (hitBorder) {
        direction = -direction;
        centipedeY++;
    } else {
        for (int& cx : centipedeX)
            cx += direction;
    }

    // Si el ciempiés llega abajo, pierdes una vida
    if (centipedeY >= HEIGHT - 1) {
        lives--;
        startCentipede();
    }

    // Si matas todo el ciempiés, aumenta dificultad
    if (centipedeX.empty()) {
        initialCount = min(initialCount + 2, WIDTH / 2);
        centipedeSpeed = max(centipedeSpeed - 5, 20);
        startCentipede();
    }
}

#ifndef _WIN32
#include <fcntl.h>
#include <stdio.h>
#include "utils.h"
#endif

// Controla el input del jugador y actualiza el juego
void updateGame() {
    if (_kbhit()) {
        int key = _getch();
        if (key == 224 || key == 0 || key == -32) {
            int arrow = _getch();
            if (arrow == 75 && playerX > 0) playerX--;         // Izquierda
            if (arrow == 77 && playerX < WIDTH - 1) playerX++; // Derecha
        } else if (key == ' ') {
            if ((int)shots.size() < MAX_SHOTS) {
                shots.push_back(Shot{playerX, HEIGHT - 2, true});
            }
        }
    }

    updateShots();

    // Mueve el ciempiés cada cierto tiempo
    if (clock() - lastmoveCentipede > centipedeSpeed * CLOCKS_PER_SEC / 1000) {
        updateCentipede();
        lastmoveCentipede = clock();
    }
}

// Retorna true si el juego terminó (sin vidas)
bool endGame() {
    return lives <= 0;
}

// Dibuja la pantalla de Game Over
void showGameOver() {
    cout << R"(  ________                                                  
 /  _____/_____    _____   ____     _______  __ ___________ 
/   \  ___\__  \  /     \_/ __ \   /  _ \  \/ // __ \_  __ \
\    \_\  \/ __ \|  Y Y  \  ___/  (  <_> )   /\  ___/|  | \/ 
 \______  (____  /__|_|  /\___  >  \____/ \_/  \___  >__|    
        \/     \/      \/     \/                   \/       )" << "\n";
}

// Menú de opciones al perder
int complete_screen_gameover() {
    vector<string> options = { "Jugar de nuevo", "Salir" };
    int selected = 0;
    char key;

    while (true) {
        clearScreenGameover();
        showGameOver();
        cout << "\n Perdiste! \n";
        cout << "\n Usa flechas (arriba/abajo) y ENTER para seleccionar:\n\n";

        for (size_t i = 0; i < options.size(); ++i) {
            cout << (i == selected ? "\t  > " : "\t    ") << options[i] << (i == selected ? " <" : "") << "\n";
        }

        key = _getch();
        if (key == -32 || key == 224 || key == 0) {
            key = _getch();
            if (key == 72) selected = (selected - 1 + (int)options.size()) % (int)options.size();
            else if (key == 80) selected = (selected + 1) % (int)options.size();
        } else if (key == 13) {
            return selected + 1;
        }
    }
}

// Función principal para iniciar el juego
void startGame() {
#ifdef _WIN32
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    cout << "\033[?25l"; 
#endif

    startTime = clock();
    playerX = WIDTH / 2;
    score = 0;
    lives = 3;
    shots.clear();
    startCentipede();

    clearScreenGameover();

    while (!endGame()) {
        clearScreenGame();
        drawScreenGame();
        showGame();
        updateGame();

#ifdef _WIN32
        Sleep(10);
#else
        usleep(10000);
#endif
    }

    guardarEstadisticas(score);

#ifdef _WIN32
    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
#else
    cout << "\033[?25h"; 
#endif

    while (true) {
        int opt = complete_screen_gameover();
        if (opt == 1) {
            startGame();
            break;
        } else {
            break;
        }
    }
}

#endif