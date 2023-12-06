#include "header.h"
#include "functions.cpp"

//Do NOT terminate the debug/run instance early because it will not delete files that needed to be removed
int main() {
    string userInput;
    startingText();
    removeShop();
    makeShop();
    recordEnemies();
    while (getline(cin, userInput)){
        if (userInput=="exit") {//if player wants to exit the game
            removeShop();
            break;
        }
        else{
            playerAction(userInput);
            if (currentHealth <= 0) break;
        }
    }
}
