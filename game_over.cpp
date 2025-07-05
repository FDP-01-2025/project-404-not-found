#include "funciones_juego.h"

HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

void enableANSI() {
    DWORD mode = 0;
    GetConsoleMode(hStdout, &mode);
    SetConsoleMode(hStdout, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void clearScreenANSI() {
    std::cout << "\x1B[2J\x1B[H";
}

void clearScreenGameover() {
    system("cls");
}

void showGameOver() {
    std::cout << R"(
 ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ 
██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗
██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝
██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║██║   ██║██╔══╝  ██╔══██╗
╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝╚██████╔╝███████╗██║  ██║
 ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚══▀▀═╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝
                                                                        
    )" << "\n";
}

int complete_screen_gameover() {
    std::vector<std::string> options = { "PLAY AGAIN", "EXIT" };
    int selected = 0;
    char key;

    while (true) {
        clearScreenGameover();
        showGameOver();
        std::cout << "\n YOU LOST THE GAME! \n";
        std::cout << "\n Use the arrow keys (up/down) and ENTER to select:\n\n";

        for (int i = 0; i < options.size(); ++i) {
            std::cout << (i == selected ? "\t  > " : "\t    ") << options[i] << (i == selected ? " <" : "") << "\n";
        }

        key = _getch();
        if (key == -32 || key == 224) {
            key = _getch();
            if (key == 72) selected = (selected - 1 + options.size()) % options.size(); // Up
            else if (key == 80) selected = (selected + 1) % options.size();             // Down
        } else if (key == 13) {
            return selected + 1;
        }
    }
}
