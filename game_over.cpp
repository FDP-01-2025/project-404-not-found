#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
using namespace std;
void limpiar_pantalla()
{
    system("cls");
}

void mostrar_pantalla_gameover()
{
    cout << R"(
 ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ 
██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗
██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝
██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║▄▄ ██║██║   ██║██╔══╝  ██╔══██╗
╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝╚██████╔╝███████╗██║  ██║
 ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚══▀▀═╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝
                                                                        
    )" << "\n";
}

int pantalla_completa_gameover()
{
    vector<string> opciones =
    {
        "JUGAR DE NUEVO",
        "SALIR"
    };
}

int seleccionado = 0;
char tecla;

while (true)
{
    limpiar_pantalla();
    mostrar_pantalla_gameover();
    cout <<"\n ¡HAS PERDIDO LA PARTIDA! \n";
    cout <<"\n Usa las flechas (arriba/abajo) y ENTER para seleccionar:\n\n";

    for (int i = 0; i < opciones.size(); i++)
    {
        if (i == seleccionado)
        {
                cout << "\t  > " << opciones[i] << " <" << "\n";
            else
                cout << "\t    " << opciones[i] << "\n";
        }
    }
    

}
