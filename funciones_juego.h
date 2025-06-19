#ifndef FUNCIONES_JUEGO_H
#define FUNCIONES_JUEGO_H

#include <vector>
#include <windows.h>
#include <ctime>

const int WIDTH = 40;
const int HEIGHT = 20;

extern char pantalla[HEIGHT][WIDTH];
extern int jugadorX;

struct Shot {
    int x, y;
    bool active;
};

extern std::vector<Shot> shots;
extern std::vector<int> ciempiesX;
extern int ciempiesY;
extern int direccion;
extern int cuentaInicial;
extern const int separacion;

extern int puntuacion;
extern int vidas;

extern int velocidadCiempies;
extern clock_t ultimaMovidaCiempies;
extern clock_t tiempoInicio;

extern HANDLE hConsole;
extern CONSOLE_CURSOR_INFO cursorInfo;

void iniciarCiempies();
void limpiarPantallaJuego();
void dibujarPantallaJuego();
void mostrarJuego();
void actualizarDisparos();
void actualizarCiempies();
void actualizarJuego();
bool juegoTerminado();
void iniciarJuego();

#endif
