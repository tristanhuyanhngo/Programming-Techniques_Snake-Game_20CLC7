#include "Header.h"

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));
    ios_base::sync_with_stdio(0);
    HiddenCursor(0);


    Menu* menu;

    while (true) {
        menu = new Menu;
        if (Run(menu) == 1) {
            system("cls");
            delete menu;
            break;
        }
        delete menu;

    }
    system("pause");
    return 0;
}