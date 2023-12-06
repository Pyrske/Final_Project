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
//string allLocations[NUM_ALL_LOCATIONS]={"mines", "dungeon"};  //all locations, including currently locked locations; unknown how many locations should have -l
//string possibleLocations[]={"mines", "dungeon"}; //all locations player can currently travel; always keep mines as first -l
//unsigned int numAvailLocations= sizeof(possibleLocations) / sizeof(possibleLocations[0]); //current number of locations the player can travel to -l
//string playerLocation=possibleLocations[0]; //current player location. At the beginning of the game, location is set to mines -l
enum location { //locations -k
    outside,
    mines,
    shop,
    dungeon
};
location currentLocation = outside; //-k

const string shopFile="shop.txt"; //-l
const string enemyFile="enemies.txt"; //-l
const unsigned int shopFileLength=getShopLength();//gets how many lines are in the shop file check with prof which method is better -l
char currentDifficulty = 0; // 0 is easy, 1 is medium, 2 is hard -k
const unsigned int NUM_ALL_ENEMIES = 4; //-k
string easyEnemies[NUM_ALL_ENEMIES], mediumEnemies[NUM_ALL_ENEMIES], hardEnemies[NUM_ALL_ENEMIES]; //-l modified my -k

int damage = 5, defense = 0, maxHealth = 100;
string weapon = "Flimsy Dagger", armor = "none";

void startingText(){ //-k
    cout << "Welcome to the game, adventurer! From here you have a few choices:\n"
            "- Enter the mines by typing 'mines'\n"
            "- Enter the shop by typing 'shop'\n"
            "- Enter the dungeon by typing 'dungeon'\n"
            "- Check your stats by typing 'stats'\n"
            "- Exit the game by typing 'exit'\n"; //-k
}

void makeShop(){//pastes the original shop into the shop file -l
    ifstream shopLines("baseShop.txt"); //-l
    ofstream toFile(shopFile); //-l
    string line; //-l

    while (getline(shopLines, line)){ //-l
        if (line.rfind("//", 0) == 0) continue; //skip comments -k
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
//                //put code to return player back to the mines area
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
    if (input == "stats"){
        playerStats(); //-k
        return; //-k
    }
    if (input == "mines"){ //-k
        cout << "Welcome to the mines! Here you can get money by typing a space\n"; //-k
        currentLocation = mines; //-k
        return; //-k
    }
}

bool addMoney(const string& input){ //-l
    if (currentLocation != mines) return false; //-k
    if (input.length() == 1 && isspace(input[0])){ //-l
        money+=baseMoneyAdd*moneyMultiplier; //-l
        cout << "Total money: " << money << "\n"; //-l
        return true; //-l
    }
    return false; //-l
}

void playerStats(){ //-k
    cout << "Your stats:\n- Health: " << maxHealth <<
         "\n- Weapon: " << weapon << " (" << damage << ")\n- Armor: " <<
         armor << " (" << defense << ")\n"; //-k
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
                getline(getShopItem, name);//gets next shop item from file -l
                getline(getShopItem, line);//gets next shop item from file -l
                if (name=="#eof" || line=="#eof") return;//prevents endless loop of item costs -l
                stringstream costLine(line); //-l
                costLine >> cost;//splits line into name and cost -l
                cout << setw(25); //-l modified by -k
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
        itemName = shopLine;
        getline(getShopItem, shopLine);
        stringstream ss(shopLine);
        int newMaxHealth = 0, newDamage = 0, newDefense = 0, healing = 0;
        ss >> itemCost >> newMaxHealth >> newDamage >> newDefense >> healing;
        if(itemToBuy == itemName) {//if the line in the txt file contains the name the user inputted -l modified by -k
            if (shopLevel > currentShopLevel){//if the item is in a shop the user has not unlocked yet -l
                cout << "The item you are trying to buy is in a locked shop\n"; //-l
                break; //-l
            }
            if (money >= itemCost) { //if the user has enough money to buy the item -l
                money -= itemCost; //update money -l
                boughtItem = true; //-l
                removeShopItem(itemName); //removes item from the shop -l
                if (newMaxHealth > maxHealth){
                    maxHealth = newMaxHealth;
                }
                if (newDamage > damage){
                    damage = newDamage;
                    weapon = itemName;
                }
                if (newDefense > defense){
                    defense = newDefense;
                    armor = itemName;
                }
                cout << "You have bought " << itemName << "\nYou have " << money << " money remaining\n"; //-k
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

void expandShop(){ //expands shop if all previous items are bought out -l
    ifstream getShopItem(shopFile);
    string line,name,pass;
    int row,col,itemCount;
    for (int shopCount=1; shopCount<=currentShopLevel; shopCount++){//modified from "openShop" function
        getline(getShopItem,line);
        stringstream shopLine(line);
        shopLine>>pass>>row>>col;
        itemCount=row*col;
        for (int i=0;i<itemCount;i++){
            getline(getShopItem,line);
            getline(getShopItem,pass);//the stat numbers that go with every item can be ignored
            if (line[0]!='#') return;//if the item is not purchased than return
        }
    }
    currentShopLevel++;//if the function was not returned then all items in the current shop level have been bought
}
//for (char i : line) { if (!(isdigit(i) || isspace(i))) return; }//if the character is part of a string(unpurchased item) return the fxn

void removeShopItem(const string& itemName){ //-l
    string lines[shopFileLength];//create an array of strings to store shop lines -k
    {
        ifstream oldShop(shopFile);//open current shop file -l modified by -k
        string line; //-l
        bool replaced = false, shopLevelCleared = true; //-k
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
            getline(oldShop, line);
            i++;
            lines[i] = line;
        }
    }//ifstream automatically closes when out of scope -k
    {
        ofstream newShop(shopFile);//override existing shop file -k
        for (int i = 0; i < shopFileLength; i++)//dump the array into the new shop file -k
            newShop << lines[i] << "\n"; //-k
    }//ofstream automatically closes when out of scope -k

    expandShop(); //-l
}

void enterDungeon(){//what happens in the dungeon -l
    char enterDungeon; //-k
    cout << "Welcome to the dungeon! Here you can fight enemies who can drop money and possibly extra supplies.\n"
            "You will fight three enemies, and then a boss fight for your rewards.\nYou will not be able to access the shop while in the dungeon\n"
            "Be careful! You only have 100 HP. Once it is depleted, it's game over!\nYou will not be able to leave once you enter.\n"
            "Do you want to enter? (y/n): "; // -k
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
            default:
                cerr<<"switch case out of range"<<endl;//-l
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
            default:
                cerr<<"switch case out of range"<<endl;//-l
        }
        enemyCounter++; //-k
    }
}