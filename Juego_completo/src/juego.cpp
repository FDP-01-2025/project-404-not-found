#include <iostream>
#include <conio.h>
#include <algorithm>
#include <vector>
#include <ctime>
#include <windows.h>
#include <fstream>

using namespace std;

// ==========================
// Constantes del juego
// ==========================
const int WIDTH = 30;
const int HEIGHT = 20;
const int MAX_SHOTS = 3;

// Estructuras
struct Shot {
    int x, y;
    bool active;
};

// Variables globales
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

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_CURSOR_INFO cursorInfo;

// ==========================
// Prototipos
// ==========================
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
void mostrarEstadisticas();

// ==========================
// Guardar estadísticas
// ==========================
void guardarEstadisticas(int nuevoPuntaje) {
    int intentos = 0;
    int mejorPuntaje = 0;

    ifstream in("puntaje.record");
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

    ofstream out("puntaje.record");
    out << "Intentos: " << intentos << endl;
    out << "MejorPuntaje: " << mejorPuntaje << endl;
    out.close();
}

// ==========================
// Mostrar estadísticas
// ==========================
void mostrarEstadisticas() {
    ifstream in("puntaje.record");
    if (in.is_open()) {
        string label1, label2;
        int intentos = 0, mejorPuntaje = 0;

        in >> label1 >> intentos;
        in >> label2 >> mejorPuntaje;

        cout << "\n====== ESTADÍSTICAS DEL JUEGO ======\n";
        cout << "Intentos: " << intentos << endl;
        cout << "Mejor puntaje: " << mejorPuntaje << endl;
        cout << "====================================\n";

        in.close();
    } else {
        cout << "\nAún no hay estadísticas guardadas.\n";
    }

    cout << "\nPresiona cualquier tecla para continuar...\n";
    _getch();
}


// ==========================
// Juego
// ==========================
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
    std::cout << R"(  ________                                                  
 /  _____/_____    _____   ____     _______  __ ___________ 
/   \  ___\__  \  /     \_/ __ \   /  _ \  \/ // __ \_  __ \
\    \_\  \/ __ \|  Y Y  \  ___/  (  <_> )   /\  ___/|  | \/ 
 \______  (____  /__|_|  /\___  >  \____/ \_/  \___  >__|    
        \/     \/      \/     \/                   \/       )" << "\n";
}

int complete_screen_gameover() {
    std::vector<std::string> options = { "PLAY AGAIN", "EXIT" };
    int selected = 0;
    char key;

    while (true) {
        clearScreenGameover();
        showGameOver();
        std::cout << "\n ¡Perdiste! \n";
        std::cout << "\n Utiliza las teclas de flecha (arriba/abajo) y ENTER para seleccionar:\n\n";

        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << (i == selected ? "\t  > " : "\t    ") << options[i] << (i == selected ? " <" : "") << "\n";
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
            startGame(); // jugar de nuevo
            break;
        } else {
            break; // salir al menú principal
        }
    }
}
