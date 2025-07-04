#include <iostream>
#include <conio.h>
using namespace std; 
int main (void){
    int x = 20;
    int y = 20;
    char tecla; 

    do
    {
        for (int i = 0; i < y; i++) //Se coloca para representar el movimiento vertical de la nave//
        {
            cout <<"\n"; 
        }
        for (int i = 0; i < x; i++) //Se coloca para realizar el movimiento horizontal de la nave//
        {
            cout <<" ";
        } 
        cout << " A "; 
        if (_kbhit()) //Se añade un if, junto con otros if anidados para realizar el movimiento de la nave, segun lo que desee realizar el jugador//
        {
            if (tecla == 'a')
            {
                x--;
            }
            if (tecla == 'd')
            {
                x++;
            }
            if (tecla == 's')
            {
                y--;
            }
            if (tecla == 'w')
            {
                y++;
            }
            else if (tecla == 27) //simboliza ESC para salir del juego//
            {
                break;
            }
            
        }
        
    } while (true);

    
    return 0;
}
