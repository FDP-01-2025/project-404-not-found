#ifndef FUNCIONES_JUEGO_H
#define FUNCIONES_JUEGO_H

#include <vector>
#include <windows.h>
#include <ctime>

const int WIDTH = 40;
const int HEIGHT = 20;

extern char pantalla[HEIGHT][WIDTH];
extern int playerX;

struct Shot {
    int x, y;
    bool active;
};

extern std::vector<Shot> shots;
extern std::vector<int> centipedeX;
extern int centipedeY;
extern int direction;
extern int initialCount;
extern const int separation;

extern int score;
extern int lives;

extern int centipedeSpeed;
extern clock_t lastmoveCentipede;
extern clock_t startTime;

extern HANDLE hConsole;
extern CONSOLE_CURSOR_INFO cursorInfo;

void startCentipede();
void clearScreenGame();
void drawScreenGame();
void showGame();
void actualizarDisparos();
void actualizarCiempies();
void actualizarJuego();
bool juegoTerminado();
void iniciarJuego();

#endif
