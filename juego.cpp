#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <algorithm> // <-- Agregado

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

using namespace std;

constexpr int WIDTH = 40;
constexpr int HEIGHT = 20;

char screen[HEIGHT][WIDTH];

int playerX = WIDTH / 2;

struct Shot {
    int x, y;
    bool active;
};
vector<Shot> shots;

vector<int> centipedeX;
int centipedeY = 0;
int direction = 1;
int initialCount = 12;
const int spacing = 3;

int score = 0;
int lives = 5; // Más vidas iniciales

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

int centipedeSpeed = 80; // ms
clock_t lastCentipedeMove = 0;
clock_t startTime;

void enableANSI() {
    DWORD mode;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void initCentipede() {
    centipedeY = 0;
    centipedeX.clear();
    for (int i = 0; i < initialCount; ++i) {
        centipedeX.push_back((i * spacing) % WIDTH);
    }
}

void clearBuffer() {
    for (int y = 0; y < HEIGHT; ++y)
        for (int x = 0; x < WIDTH; ++x)
            screen[y][x] = ' ';
}

void drawBuffer() {
    screen[HEIGHT - 1][playerX] = '^';
    for (const auto& shot : shots) {
        if (shot.active && shot.y >= 0 && shot.y < HEIGHT)
            screen[shot.y][shot.x] = '|';
    }
    for (int cx : centipedeX) {
        if (centipedeY < HEIGHT)
            screen[centipedeY][cx] = 'O';
    }
}

void render() {
    cout << "\x1B[H";
    cout << "Score: " << score << "    Lives: " << lives << "\n";
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            char c = screen[y][x];
            switch (c) {
            case '^':
                cout << "\x1B[32m" << c << "\x1B[0m";
                break;
            case '|':
                cout << "\x1B[33m" << c << "\x1B[0m";
                break;
            case 'O':
                cout << "\x1B[31m" << c << "\x1B[0m";
                break;
            default:
                cout << c;
            }
        }
        cout << "\n";
    }
    cout.flush();
}

void updateShots() {
    for (auto& shot : shots) {
        if (shot.active) {
            shot.y--;
            if (shot.y < 0) shot.active = false;
        }
    }
    // Colisión disparo-centipede
    for (auto& shot : shots) {
        if (!shot.active) continue;
        for (size_t i = 0; i < centipedeX.size(); ++i) {
            if (centipedeY == shot.y && centipedeX[i] == shot.x) {
                centipedeX.erase(centipedeX.begin() + i);
                shot.active = false;
                score += 10;
                break;
            }
        }
    }
    // Eliminar disparos inactivos
    shots.erase(remove_if(shots.begin(), shots.end(), [](const Shot& s) { return !s.active; }), shots.end());
}

void updateCentipede() {
    bool hitEdge = false;
    for (int cx : centipedeX) {
        int nx = cx + direction;
        if (nx < 0 || nx >= WIDTH) {
            hitEdge = true;
            break;
        }
    }
    if (hitEdge) {
        direction = -direction;
        centipedeY++;
    }
    else {
        for (int& cx : centipedeX)
            cx += direction;
    }
    if (centipedeY >= HEIGHT - 1) {
        lives--;
        initCentipede();
    }
    if (centipedeX.empty()) {
        // Aumenta dificultad al reaparecer
        initialCount = min(initialCount + 2, WIDTH / 2);
        centipedeSpeed = max(centipedeSpeed - 5, 20); // Más rápido
        initCentipede();
    }
}

void updateDifficulty() {
    // Dificultad basada en score y tiempo
    int elapsed = (clock() - startTime) / CLOCKS_PER_SEC;
    centipedeSpeed = max(20, 80 - score / 20 - elapsed / 5);
}

void update() {
    if (_kbhit()) {
        int c = _getch();
        if (c == 224) {
            int arrow = _getch();
            if (arrow == 75 && playerX > 0)
                playerX--;
            else if (arrow == 77 && playerX < WIDTH - 1)
                playerX++;
        }
        else if (c == ' ') {
            // Disparo rápido, hasta 3 disparos en pantalla
            if (shots.size() < 3)
                shots.push_back({ playerX, HEIGHT - 2, true });
        }
    }
    updateShots();
    updateDifficulty();
    // Mueve el centipede según velocidad
    if (clock() - lastCentipedeMove > centipedeSpeed * CLOCKS_PER_SEC / 1000) {
        updateCentipede();
        lastCentipedeMove = clock();
    }
}

bool isGameOver() {
    return lives <= 0;
}

int main() {
    enableANSI();
    cout << "\x1B[?25l" << "\x1B[2J";
    startTime = clock();
    initCentipede();
    while (!isGameOver()) {
        clearBuffer();
        drawBuffer();
        render();
        update();
        Sleep(10); // Más fluido
    }
    cout << "\x1B[?25h";
    cout << "\n\n\x1B[31mGAME OVER\x1B[0m   Final Score: " << score << "\n";
    return 0;
}