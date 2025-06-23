#include <iostream>
#include <conio.h>
#include <algorithm>
#include "funciones_juego.h"

using namespace std;

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

void startCentipede() {
    centipedeY = 0;
    centipedeX.clear();
    for (int i = 0; i < initialCount; i++) {
        centipedeX.push_back((i * separation) % WIDTH);
    }
}

void clearScreenGame() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            pantalla[y][x] = ' ';
        }
    }
}

void drawScreenGame() {
    pantalla[HEIGHT - 1][playerX] = '^';

    for (auto& shot : shots) {
        if (shot.active && shot.y >= 0 && shot.y < HEIGHT) {
            pantalla[shot.y][shot.x] = '|';
        }
    }

    for (int cx : centipedeX) {
        if (centipedeY < HEIGHT) {
            pantalla[centipedeY][cx] = 'O';
        }
    }
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
        for (int x = 0; x < WIDTH; x++) {
            cout << pantalla[y][x];
        }
        cout << "|" << endl;
    }

    cout << "+";
    for (int i = 0; i < WIDTH; i++) cout << "-";
    cout << "+" << endl;
}

void updateShots() {
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
        for (int j = 0; j < centipedeX.size(); j++) {
            if (centipedeY == shots[i].y && centipedeX[j] == shots[i].x) {
                centipedeX.erase(centipedeX.begin() + j);
                shots[i].active = false;
                score += 10;
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

void updateCentipede() {
    bool chocoBorde = false;
    for (int cx : centipedeX) {
        int nuevoX = cx + direction;
        if (nuevoX < 0 || nuevoX >= WIDTH) {
            chocoBorde = true;
            break;
        }
    }

    if (chocoBorde) {
        direction = -direction;
        centipedeY++;
    } else {
        for (int& cx : centipedeX) {
            cx += direction;
        }
    }

    if (centipedeY >= HEIGHT - 1) {
        lives--;
        startCentipede();
    }

    if (centipedeX.empty()) {
        initialCount += 2;
        if (initialCount > WIDTH / 2) initialCount = WIDTH / 2;

        centipedeSpeed -= 5;
        if (centipedeSpeed < 20) centipedeSpeed = 20;

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
            if (shots.size() < 3) {
                Shot newShot;
                newShot.x = playerX;
                newShot.y = HEIGHT - 2;
                newShot.active = true;
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

    cursorInfo.bVisible = true;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    system("cls");
    cout << "\n=== JUEGO TERMINADO ===\n";
    cout << "Puntaje final: " << score << endl;
    system("pause");
}
