#include <iostream>
#include <string>
using namespace std;

int player_selection(int option){
    cout << "¡Bienvenido a Centipede!, por favor escoge una opcion:\n";
    cout << "1 - INICIAR NUEVA PARTIDA\n2 - SALIR DEL JUEGO\n";
    cin >> option;

    while(option != 1 && option !=2){

        option = 0;
        cout << "INGRESA UN VALOR VALIDO\n";
        cin >> option;
    }

    switch(option){
            
            case 1:
                cout << "CARGANDO PARTIDA...\n";
                break;
            case 2:
                cout << "GRACIAS POR JUGAR\n";
                break;

    return option;
    }
}


int main() {
    int option;
cout << R"(
=====================================================
  ____ _____ _   _ _____ ___ ____  _____ ____  _____ 
 / ___| ____| \ | |_   _|_ _|  _ \| ____|  _ \| ____|
| |   |  _| |  \| | | |  | || |_) |   | | | | |  _| 
| |___| |___| |\  | | |  | ||  __/| |___| |_| | |___ 
 \____|_____|_| \_| |_| |___|_|   |_____|____/|_____|

=====================================================
 )" << "\n";


player_selection(option);

   return 0;
}
