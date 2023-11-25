#include "header.h"
#include "functions.cpp"

int main() {
    string userInput;
    startingText();
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
