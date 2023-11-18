#include "header.h"

int main() {
    string userInput;
    makeShop();
    recordEnemies();
    while (getline(cin, userInput)){
        if (userInput=="exit") {//if player wants to exit the game
            removeShop();
            break;
        }
        else{
            playerAction(userInput);
        }
    }
}
