//
// Created by liamg on 10/31/2023.
// Note: code mrked with a -l is Liam's, and code marked with a -k is Kalib's
//
#include "header.h"

unsigned long long int money=0; //-l
long long int baseMoneyAdd=1; //the base amount of money you will get by pressing space -k
long long int moneyMultiplier=1; //multiplies base money to get total money every time you press space -k
int currentShopLevel=1; //-l

//const unsigned int NUM_ALL_LOCATIONS=2; //-l
//string allLocations[NUM_ALL_LOCATIONS]={"outside", "dungeon"};  //all locations, including currently locked locations; unknown how many locations should have -l
//string possibleLocations[]={"outside", "dungeon"}; //all locations player can currently travel; always keep outside as first -l
//unsigned int numAvailLocations= sizeof(possibleLocations) / sizeof(possibleLocations[0]); //current number of locations the player can travel to -l
//string playerLocation=possibleLocations[0]; //current player location. At the beginning of the game, location is set to outside -l
enum location { //locations -k
    outside,
    shop,
    dungeon
};
location currentLocation = outside; //-k

const string shopFile="shop.txt"; //-l
const string enemyFile="enemies.txt"; //-l
const unsigned int shopFileLength=getShopLength();//gets how many lines are in the shop file check with prof which method is better -l
bool inShop = false; //-k
char currentDifficulty = 0; // 0 is easy, 1 is medium, 2 is hard -k
const unsigned int NUM_ALL_ENEMIES = 4; //-k
string easyEnemies[NUM_ALL_ENEMIES], mediumEnemies[NUM_ALL_ENEMIES], hardEnemies[NUM_ALL_ENEMIES]; //-l modified my -k


void startingText(){ //-k
    cout << "Welcome to the game, adventurer! From here you have three choices:\n- Enter the shop by typing 'shop'\n"
            "- Enter the dungeon by typing 'dungeon'\n- Exit the game by typing 'exit'\nEverytime you type a space and "
            "hit enter, you will get one money\n"; //-k
}

void makeShop(){//pastes the original shop into the shop file -l
    ifstream shopLines("baseShop.txt"); //-l
    ofstream toFile(shopFile); //-l
    string line; //-l

    while (getline(shopLines, line)){ //-l
        toFile << line << "\n"; //-l
    }
}

void removeShop(){//removes shop file before code ends so duplicate files don't appear after multiple runs -l
    remove(shopFile.c_str()); //-l
}


void playerAction(const string& input){ //-l
    if (addMoney(input)) //-l
        return; //-l
//    if (input == "location"){
//        inShop = false;
//        changeLocation(input);
//        switch (playerLocation) {
//            case 0:
//                //put code to return player back to the outside area
//                break;
//        }
//    }
    if (input == "shop"){ //-l
        currentLocation = shop; //-k
        openShop(); //-l
        return; //-l
    }
    if (input == "buy"){ //-l modified by -k
        if (currentLocation == shop) buyItem(); //-k
        else cout << "You must be in the shop to buy items\n"; //-k
        return; //-k
    }
    if (input == "dungeon"){ //-k
        currentLocation = dungeon; //-k
        enterDungeon(); //-k
        return; //-k
    }
}

bool addMoney(const string& input){ //-l
    if (input.length() == 1 && isspace(input[0])){ //-l
        money+=baseMoneyAdd*moneyMultiplier; //-l
        cout << "Total money: " << money << "\n"; //-l
        return true; //-l
    }
    return false; //-l
}

bool changeLocation(const string& input){ //-l
//    string newLocation; //-l
//    cout << "Current Location: " << playerLocation << "\n" << "Where would you like to go? "; //-l modified slightly by -k
//    cin >> newLocation; //-l
//    cout << "\n"; //-l
//
//    for (char i : input){ //loops through chars in the string, making all characters lowercase -l
//        if (i>=65 && i<=90) //-l
//            i+=32; //-l
//    }
//    if (input == newLocation)//if player is trying to go to a place where they already are -l
//        return false; //-l
//    for (const string& location : possibleLocations){ //loops through the possible locations, setting the new player location to user inputted if it is a possible location to travel to -l
//        if (location  ==  input){ //-l
//            playerLocation = location; //-l
//            cout << "New Location: " << playerLocation << "\n"; //tells player their new location -l
//            return true; //add statement that triggers depending on what location they are now in -l
//        }
//    }
    return false; //-l
}

unsigned int getShopLength(){ //-l
    ifstream shopLines("baseShop.txt"); //-l
    string line; //-l
    unsigned int counter=0; //-l
    while (getline(shopLines, line)){ //-l
        counter++; //-l
    }
    return counter; //-l
}

void openShop() { //-l
    ifstream getShopItem(shopFile); //-l
    string line, start, item, name, cost; //-l
    int rows,columns; //-l
    cout << "Type 'buy' to purchase an item\nCurrent Shop Level: " << currentShopLevel << "\n"; //-k
    for (int shopCount=1; shopCount<=currentShopLevel; shopCount++){ //outputs the number of shops equal to their level -l
        getline(getShopItem, line);//gets shop statement -l
        stringstream ss(line); //-l
        ss >> start >> rows >> columns; //separates the line into the "#shop", width and height -l
        for (int currentRow=0; currentRow < rows; currentRow++){ //-l modified by -k
            for (int currentCol=0; currentCol < columns; currentCol++) { //for the size of the shop print out appropriate names and spaces in the console modified by -k
                getline(getShopItem, line);//gets next shop item from file -l
                stringstream shopLine(line); //-l
                shopLine >> name >> cost;//splits line into name and cost -l
                cout << setw(15); //-l modified by -k
                if (name == "#purchased") { //if the item is purchased, leave a sold message where the item usually goes -l modified by -k
                    cout << "-SOLD-"; //-k
                } else { //-k
                    item = name + " (" + cost + ")"; //-k
                    cout << item; //prints next item with space -k
                }
            }
            cout << setw(0) << "\n"; //-k
        }
    }
}

void buyItem(){ //-l
    ifstream getShopItem(shopFile); //-l
    string itemToBuy,shopLine,itemName; //-l
    int shopLevel=0,itemCost;//shop level starts at 0 b/c the first line of shop file adds one level -l
    bool boughtItem=false; //-k
    cout << "What item would you like to buy: "; //-l
    getline(cin,itemToBuy); //gets item user would like to buy -l
    if (itemToBuy[0] == '#'){ //-k
        cout << "This item does not exist\n"; //-k
        return; //-k
    }
    while (getline(getShopItem, shopLine)){ //while there is a line to get from the file -l
        if (shopLine.find("#shop") != string::npos){ //if the line that is being read is a shop header, continue and add one to the shop level -l
            shopLevel++; //-l
            continue; //-l
        }
        stringstream ss(shopLine); //split the line into the name and the cost -l
        ss >> itemName >> itemCost; //-l
        if(itemToBuy == itemName) {//if the line in the txt file contains the name the user inputted -l modified by -k
            if (shopLevel > currentShopLevel){//if the item is in a shop the user has not unlocked yet -l
                cout << "The item you are trying to buy is in a locked shop\n"; //-l
                break; //-l
            }
            if (money >= itemCost) { //if the user has enough money to buy the item -l
                cout << "You have bought " << itemName << "\n"; //-l
                money -= itemCost; //update money -l
                cout << "You have " << money << " money remaining\n"; //-k
                boughtItem = true; //-l
                removeShopItem(itemName); //removes item from the shop -l
                break; //-l
            } else { //-k
                cout << "You do not have enough money to buy this item\n"; //-k
                break; //-k
            }
        }
    }
    if (!boughtItem) //-k
        cout << "This item does not exist\n"; //-k
}

void removeShopItem(const string& itemName){ //-l
    string lines[shopFileLength];//create an array of strings to store shop lines -k
    {
        ifstream oldShop(shopFile);//open current shop file -l modified by -k
        string line; //-l
        bool replaced = false, shopLevelCleared = true; //-l
        int shopLevel = 0; //-l
        int i = -1; //for the array -k
        while (getline(oldShop, line)){//go over lines from old shop -k
            i++; //-k
            if (line.find("#shop") != string::npos){//check if line has #shop -k
                if (shopLevelCleared && shopLevel == currentShopLevel)//check if previous shop level has been cleared and if the player is on that level -k
                    currentShopLevel++; //-k
                shopLevelCleared = true; //-k
                lines[i] = line;//copy the current line to the array -k
                shopLevel++; //-k
                continue; //-k
            }
            if (line.find(itemName) != string::npos && !replaced){//check if line has the target item and if it has not been replaced -k
                lines[i] = "#purchased";//tag item as purchased -k
                replaced = true; //-k
                continue; //-k
            }
            if (line.find("#purchased") == string::npos)//check if item hasn't been purchased -k
                shopLevelCleared = false;//mark current level as not cleared -k
            lines[i] = line; //-k
        }
    }//ifstream automatically closes when out of scope -k
    {
        ofstream newShop(shopFile);//override existing shop file -k
        for (int i = 0; i < shopFileLength; i++)//dump the array into the new shop file -k
            newShop << lines[i] << "\n"; //-k
    }//ofstream automatically closes when out of scope -k
}

void enterDungeon(){//what happens in the dungeon -l
    char enterDungeon; //-k
    cout << "Welcome to the dungeon! Here you can fight enemies who can drop money and possibly extra supplies.\n"
            "You will fight three enemies, and then a boss fight for your rewards.\nYou will not be able to access the shop while in the dungeon\n"
            "Be careful! You only have three hearts. Once all three are depleted, it's game over!\nDo you want to enter? (y/n): "; // -k
    cin >> enterDungeon; //-k
    if (tolower(enterDungeon) == 'n'){ //-k
        cout << "exiting the dungeon\n"; //-k
        return; //-k
    }
    if (tolower(enterDungeon) != 'y') //-k
        cout << "improper input"; //-k
    for (int i = 0; i < 3; i++){ //-k
        string currentEnemy; //-k
        switch(currentDifficulty){ //-k
            case 0: //easy difficulty -k
                currentEnemy = easyEnemies[i]; //-k
                break; //-k
            case 1: //medium difficulty -k
                currentEnemy = mediumEnemies[i]; //-k
                break; //-k
            case 2: //hard difficulty -k
                currentEnemy = hardEnemies[i]; //-k
                break; //-k
        }
    }


}

unsigned int difficultyLine(const string& difficulty){ //-l
    string line; //-l
    unsigned int counter=0; //-l
    ifstream fin(enemyFile); //-l
    while(getline(fin, line)){ //-l
        counter++; //-l
        if (line.find(difficulty)!=string::npos) { //-l
            break; //-l
        }
    }
    return counter; //-l
}

void recordEnemies(){ //-l
    ifstream enemies(enemyFile); //-k
    string line, enemyString, enemyStats; //-k
    int difficultyCounter = -1, enemyCounter = 0; //-k

    while (getline(enemies, line)) { //-k
        if (line.rfind("//", 0) == 0) continue; //skip comments -k

        if (line.find("#difficulty") != string::npos){ //-k
            difficultyCounter++; //-k
            enemyCounter = 0; //-k
            continue; //-k
        }
        if (line.rfind("#B ", 0) == 0) { //-k
            line.erase(0, 3); //-k

        }
        enemyString = line; //-k
        getline(enemies, enemyStats); //-k
        enemyString += "\n" + enemyStats; //-k
        switch (difficultyCounter){ //-k
            case 0: //-k
                easyEnemies[enemyCounter] = enemyString; //-k
                break; //-k
            case 1: //-k
                mediumEnemies[enemyCounter] = enemyString; //-k
                break; //-k
            case 2: //-k
                hardEnemies[enemyCounter] = enemyString; //-k
                break; //-k
        }
        enemyCounter++; //-k
    }
}



//const unsigned int easyLineLocation= getEnemyDifficultyLine("Easy"),mediumLineLocation= difficultyLine("Medium"),
//hardLineLocation= getEnemyDifficultyLine("Hard"),bossLineLocation= difficultyLine("Boss");
