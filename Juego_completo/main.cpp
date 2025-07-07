#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <direct.h>
#include "./src/menu.h"

using namespace std;

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define DIR_NAME "informe"
#define FILE_PATH "informe/puntaje.record"

const int WIDTH = 30;
const int HEIGHT = 20;
const int MAX_SHOTS = 3;

HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO cursorInfo;

char pantalla[HEIGHT][WIDTH];
int playerX = WIDTH / 2;
vector<struct Shot> shots;
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

struct Shot {
    int x, y;
    bool active;
};

// Declaraciones internas para funciones de juego
void startCentipede();
void clearScreenGame();
void drawScreenGame();
void showGame();
void updateShots();
void updateCentipede();
void updateGame();
bool endGame();
void clearScreenGameover();
void showGameOver();
int complete_screen_gameover();
void guardarEstadisticas(int nuevoPuntaje);

// -------------- Implementaciones ------------------

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

void guardarEstadisticas(int nuevoPuntaje) {
    _mkdir(DIR_NAME);

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

void startCentipede() {
    centipedeY = 0;
    centipedeX.clear();
    for (int i = 0; i < initialCount; i++) {
        centipedeX.push_back((i * separation) % WIDTH);
    }
}

void clearScreenGame() {
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            pantalla[y][x] = ' ';
}

void drawScreenGame() {
    pantalla[HEIGHT - 1][playerX] = '^';
    for (const auto& shot : shots)
        if (shot.active && shot.y >= 0 && shot.y < HEIGHT)
            pantalla[shot.y][shot.x] = '|';

    for (int cx : centipedeX)
        if (centipedeY < HEIGHT)
            pantalla[centipedeY][cx] = 'O';
}

void showGame() {
    COORD pos = {0, 0};
    SetConsoleCursorPosition(hConsole, pos);

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
}

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

    if (centipedeY >= HEIGHT - 1) {
        lives--;
        startCentipede();
    }

    if (centipedeX.empty()) {
        initialCount = min(initialCount + 2, WIDTH / 2);
        centipedeSpeed = max(centipedeSpeed - 5, 20);
        startCentipede();
    }
}

void updateGame() {
    if (_kbhit()) {
        int key = _getch();
        if (key == 224) {
            int arrow = _getch();
            if (arrow == 75 && playerX > 0) playerX--;
            if (arrow == 77 && playerX < WIDTH - 1) playerX++;
        } else if (key == ' ') {
            if (shots.size() < MAX_SHOTS) {
                Shot newShot{playerX, HEIGHT - 2, true};
                shots.push_back(newShot);
            }
        }
    }

    updateShots();

    if (clock() - lastmoveCentipede > centipedeSpeed * CLOCKS_PER_SEC / 1000) {
        updateCentipede();
        lastmoveCentipede = clock();
    }
}

bool endGame() {
    return lives <= 0;
}

void clearScreenGameover() {
    system("cls");
}

void showGameOver() {
    cout << R"(  ________                                                  
 /  _____/_____    _____   ____     _______  __ ___________ 
/   \  ___\__  \  /     \_/ __ \   /  _ \  \/ // __ \_  __ \
\    \_\  \/ __ \|  Y Y  \  ___/  (  <_> )   /\  ___/|  | \/ 
 \______  (____  /__|_|  /\___  >  \____/ \_/  \___  >__|    
        \/     \/      \/     \/                   \/       )" << "\n";
}

int complete_screen_gameover() {
    vector<string> options = { "Jugar de nuevo", "Salir" };
    int selected = 0;
    char key;

    while (true) {
        clearScreenGameover();
        showGameOver();
        cout << "\n Perdiste! \n";
        cout << "\n Utiliza las teclas de flecha (arriba/abajo) y ENTER para seleccionar:\n\n";

        for (size_t i = 0; i < options.size(); ++i) {
            cout << (i == selected ? "\t  > " : "\t    ") << options[i] << (i == selected ? " <" : "") << "\n";
        }

        key = _getch();
        if (key == -32 || key == 224) {
            key = _getch();
            if (key == 72) selected = (selected - 1 + options.size()) % options.size();
            else if (key == 80) selected = (selected + 1) % options.size();
        } else if (key == 13) {
            return selected + 1;
        }
    }
}

void startGame() {
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    startTime = clock();
    playerX = WIDTH / 2;
    score = 0;
    lives = 3;
    shots.clear();
    startCentipede();

    system("cls");

    while (!endGame()) {
        clearScreenGame();
        drawScreenGame();
        showGame();
        updateGame();
        Sleep(10);
    }

    guardarEstadisticas(score);

    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

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

        _getch(); // espera tecla para volver al menú
    }

    return 0;
}
