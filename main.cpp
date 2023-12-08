//Author: Kalib LaClair and Liam Gugliotta
//Class: CSI-140-06
//Assignment: Final Project
//Date Assigned: 11/06/2023
//Due Date: 12/10/2023 11:59 PM
//
//Description:
//Text-based Game
//
//Certification of Authenticity:
//I certify that this is entirely my own work, except where I have given
//fully-documented references to the work of others. I understand the
//definition and consequences of plagiarism and acknowledge that the assessor
//of this assignment may, for the purpose of assessing this assignment:
//        - Reproduce this assignment and provide a copy to another member of
//          academic staff; and/or
//        - Communicate a copy of this assignment to a plagiarism checking
//          service (which may then retain a copy of this assignment on its
//          database for the purpose of future plagiarism checking)
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
            if (!playerAction(userInput)) break;
        }
    }
}
