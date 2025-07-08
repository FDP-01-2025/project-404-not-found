//archivo .h para usarse en un ambiente de Linux
#ifndef UTILS_H
#define UTILS_H

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdio.h>
// Detecta si se ha presionado una tecla
int _kbhit() {
    struct timeval tv = {0, 0};
    fd_set read_fd;
    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);
    if (select(1, &read_fd, NULL, NULL, &tv) == -1)
        return 0;
    return FD_ISSET(0, &read_fd);
}
// Lee una tecla del teclado sin esperar ENTER y sin mostrarla en pantalla
int _getch() {
    struct termios oldt, newt;
    int ch = 0;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    if (ch == 27) { // ESC
        if (_kbhit() && getchar() == '[') {
            int dir = getchar();
            switch (dir) {
                /*son las flechas del tecladp
                que controla al personaje del juego*/
                case 'A': ch = 72; break; 
                case 'B': ch = 80; break; 
                case 'C': ch = 77; break; 
                case 'D': ch = 75; break; 
            }
        }
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

#endif 