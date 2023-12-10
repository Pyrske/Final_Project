//
// Created by liamg on 10/31/2023.
// Note: all code after a mark with -l is Liam's, and all code after a mark with -k is Kalib's
//
//-l
#include "header.h"

unsigned long long int money = 0;
long long int baseMoneyAdd = 1; //the base amount of money you will get by pressing space

//-k
enum location { //locations
    outside,
    mines,
    shop,
    dungeon
};
location currentLocation = outside; //default location

//-l
int currentShopLevel = 1;
const string shopFile = "shop.txt";
const string enemyFile = "enemies.txt";
const unsigned int shopFileLength = getShopLength();//gets how many lines are in the shop file check with prof which method is better

//-k
int currentDifficulty = 0; // 0 is easy, 1 is medium, 2 is hard
const unsigned int NUM_ALL_ENEMIES = 4;
//-l
string easyEnemies[NUM_ALL_ENEMIES], mediumEnemies[NUM_ALL_ENEMIES], hardEnemies[NUM_ALL_ENEMIES];

//-k
int damage = 5, defense = 0, maxHealth = 100, currentHealth = 100;
string weapon = "Flimsy Dagger", armor = "none", accessory = "none"; //starting items

const unsigned int SMALL_HEALING = 25, MEDIUM_HEALING = 50, LARGE_HEALING = 75; //amount health potions of varying sizes will heal
int smallHealthPotionCount = 0, mediumHealthPotionCount = 0, largeHealthPotionCount = 0; //number of health potions the player has

const unsigned int SMALL_STRENGTH = 20, LARGE_STRENGTH = 40; //amount of strength the strength potions will apply
int smallStrengthPotionCount = 0, largeStrengthPotionCount = 0; //number of strength potions the player has

void startingText() {
    cout << "Welcome to the game, adventurer! From here you have a few choices:\n"
            "- Enter the mines by typing 'mines'\n"
            "- Enter the shop by typing 'shop'\n"
            "- Enter the dungeon by typing 'dungeon'\n"
            "- Check your inventory by typing 'inventory'\n"
            "- Exit the game by typing 'exit'\n"; //text the prints at the start of the game
}

//-l
void makeShop() {//pastes the original shop into the shop file
    ifstream shopLines("baseShop.txt");
    ofstream toFile(shopFile);
    string line;

    while (getline(shopLines, line)) {
        //-k
        if (line.rfind("//", 0) == 0) continue; //skip comments
        //-l
        toFile << line << "\n";
    }
}

void removeShop() {//removes shop file before code ends so duplicate files don't appear after multiple runs
    remove(shopFile.c_str());
}

bool playerAction(const string &input) {
    if (addMoney(input))
        return true;
    if (input == "shop") {
        //-k
        currentLocation = shop;
        //-l
        openShop();
        return true;
    }
    //-l
    if (input == "buy") {
        //-k
        if (currentLocation == shop) buyItem(); //makes it so the player can only buy shop items while in the shop
        else cout << "You must be in the shop to buy items\n";
        return true;
    }
    //sends the player to the location they entered minus the Easter egg and inventory
    if (input == "dungeon") {
        currentLocation = dungeon;
        return enterDungeon();
    }
    if (input == "inventory") {
        playerInventory();
        return true;
    }
    if (input == "mines") {
        cout << "Welcome to the mines! Here you can get money by typing a space\n";
        currentLocation = mines;
        return true;
    }
    if (input == "ping") {//this is just a silly Easter egg
        cout << "pong!\n";
        return true;
    }
    return true;
}

//-l
bool addMoney(const string &input) {
    //-k
    if (currentLocation != mines) return false; //makes it so money can only be gained in the mines
    //-l
    if (input.length() == 1 && isspace(input[0])) { //each space gives one coin
        money += baseMoneyAdd;
        cout << "Total money: " << money << "\n";
        return true;
    }
    return false;
}

//-k
void playerInventory() {
    cout << "Your inventory:\n- Money: " << money <<
         "\n- Accessory: " << accessory << " (" << maxHealth << " HP)\n- Weapon: " <<
         weapon << " (" << damage << ")\n- Armor: " <<
         armor << " (" << defense << ")\n"; //lists everything in the player's inventory
    if (smallHealthPotionCount + mediumHealthPotionCount + largeHealthPotionCount + smallStrengthPotionCount +
        largeStrengthPotionCount <= 0) {
        cout << "You have no potions\n";
        return;
    }
    cout << "Your potions:\n";
    if (smallHealthPotionCount > 0)
        cout << "- Small Healing Potion: " << smallHealthPotionCount << "\n";
    if (mediumHealthPotionCount > 0)
        cout << "- Medium Healing Potion: " << mediumHealthPotionCount << "\n";
    if (largeHealthPotionCount > 0)
        cout << "- Large Healing Potion: " << largeHealthPotionCount << "\n";
    if (smallStrengthPotionCount > 0)
        cout << "- Small Strength Potion: " << smallStrengthPotionCount << "\n";
    if (largeStrengthPotionCount > 0)
        cout << "- Large Strength Potion: " << largeStrengthPotionCount << "\n";
} //only lists potions if there is more than zero
//-l
unsigned int getShopLength() {
    ifstream shopLines("baseShop.txt");
    string line;
    unsigned int counter = 0;
    while (getline(shopLines, line)) {
        counter++;
    }
    return counter;
}

void openShop() {
    ifstream getShopItem(shopFile);
    string line, start, item, name, cost;
    int rows, columns;
    //-k
    cout << "Type 'buy' to purchase an item\nCurrent Shop Level: " << currentShopLevel << "\n";
    //-l
    for (int shopCount = 1; shopCount <= min(currentShopLevel, currentDifficulty +
                                                               1); shopCount++) { //outputs the number of shops equal to their level
        getline(getShopItem, line);//gets shop statement
        stringstream ss(line);
        ss >> start >> rows >> columns; //separates the line into the "#shop", width and height
        for (int currentRow = 0; currentRow < rows; currentRow++) {
            //-k
            for (int currentCol = 0; currentCol <
                                     columns; currentCol++) { //for the size of the shop print out appropriate names and spaces in the console modified by
                //-l
                getline(getShopItem, name);//gets next shop item from file
                if (name == "#eof") return;//prevents endless loop of item costs
                getline(getShopItem, line);//gets next shop item from file
                stringstream costLine(line);
                costLine >> cost;//splits line into name and cost
                cout << setw(25);
                if (name == "#purchased") { //if the item is purchased, leave a sold message where the item usually goes
                    //-k
                    cout << "-SOLD-";
                } else {
                    item = name + " (" + cost + ")";
                    cout << item; //prints next item with space
                }
            }
            cout << setw(0) << "\n";
        }
    }
}

//-l
void buyItem() {
    ifstream getShopItem(shopFile);
    string itemToBuy, shopLine, itemName;
    int shopLevel = 0, itemCost;//shop level starts at 0 b/c the first line of shop file adds one level
    //-k
    bool boughtItem = false;
    //-l
    cout << "What item would you like to buy: ";
    getline(cin, itemToBuy); //gets item user would like to buy
    //-k
    if (itemToBuy[0] == '#') {
        cout << "This item does not exist\n";
        return;
    }
    //-l
    while (getline(getShopItem, shopLine)) { //while there is a line to get from the file
        if (shopLine.find("#shop") !=
            string::npos) { //if the line that is being read is a shop header, continue and add one to the shop level
            shopLevel++;
            continue;
        }
        //-l
        itemName = shopLine;
        getline(getShopItem, shopLine);
        stringstream ss(shopLine);
        //-k
        int newMaxHealth = 0, newDamage = 0, newDefense = 0, potionType = 0;
        ss >> itemCost >> newMaxHealth >> newDamage >> newDefense >> potionType;
        //-l
        if (itemToBuy == itemName) {//if the line in the txt file contains the name the user inputted
            if (shopLevel > currentShopLevel) {//if the item is in a shop the user has not unlocked yet
                cout << "The item you are trying to buy is in a locked shop\n";
                break;
            }
            if (money >= itemCost) { //if the user has enough money to buy the item
                money -= itemCost; //update money
                boughtItem = true;
                removeShopItem(itemName); //removes item from the shop
                //-k
                //when a shop item is bought that is equippable, that item is equipped and its stats are added to the player
                if (newMaxHealth > maxHealth) {
                    maxHealth = newMaxHealth;
                    accessory = itemName;
                }
                if (newDamage > damage) {
                    damage = newDamage;
                    weapon = itemName;
                }
                if (newDefense > defense) {
                    defense = newDefense;
                    armor = itemName;
                }
                //when a potion is bought it is added to the inventory
                switch (potionType) {
                    case 1:
                        smallHealthPotionCount++;
                        break;
                    case 2:
                        mediumHealthPotionCount++;
                        break;
                    case 3:
                        largeHealthPotionCount++;
                        break;
                    case 4:
                        smallStrengthPotionCount++;
                        break;
                    case 5:
                        largeStrengthPotionCount++;
                        break;
                }
                cout << "You have bought " << itemName << "\nYou have " << money << " money remaining\n";
                //-l
                break;
                //-k
            } else {
                cout << "You do not have enough money to buy this item\n";
                return;//break also causes "This item does not exist" to cout
            }
        }
    }
    if (!boughtItem)
        cout << "This item does not exist\n"; //for mistypes
}

//-l
void expandShop() { //expands shop if all previous items are bought out
    ifstream getShopItem(shopFile);
    string line, name, pass;
    int row, col, itemCount;
    for (int shopCount = 1; shopCount <= currentShopLevel; shopCount++) {//modified from "openShop" function
        getline(getShopItem, line);
        stringstream shopLine(line);
        shopLine >> pass >> row >> col;
        itemCount = row * col;
        for (int i = 0; i < itemCount; i++) {
            getline(getShopItem, line);
            getline(getShopItem, pass);//the stat numbers that go with every item can be ignored
            if (line[0] != '#') return;//if the item is not purchased than return
        }
    }
    currentShopLevel++;//if the function was not returned then all items in the current shop level have been bought
}
//for (char i : line) { if (!(isdigit(i) || isspace(i))) return; }//if the character is part of a string(unpurchased item) return the fxn

void removeShopItem(const string &itemName) {
    //-k
    string lines[shopFileLength];//create an array of strings to store shop lines
    {
        //-l
        ifstream oldShop(shopFile);//open current shop file
        string line;
        //-k
        bool replaced = false, shopLevelCleared = true;
        //-l
        int shopLevel = 0;
        //-k
        int i = -1; //for the array
        while (getline(oldShop, line)) {//go over lines from old shop
            i++;
            if (line.find("#shop") != string::npos) {//check if line has #shop
                if (shopLevelCleared && shopLevel ==
                                        currentShopLevel)//check if previous shop level has been cleared and if the player is on that level
                    currentShopLevel++;
                shopLevelCleared = true;
                lines[i] = line;//copy the current line to the array
                shopLevel++;
                continue;
            }
            if (line.find(itemName) != string::npos &&
                !replaced) {//check if line has the target item and if it has not been replaced
                lines[i] = "#purchased";//tag item as purchased
                replaced = true;
                continue;
            }
            if (line.find("#purchased") == string::npos)//check if item hasn't been purchased
                shopLevelCleared = false;//mark current level as not cleared
            lines[i] = line;
            getline(oldShop, line);
            i++;
            lines[i] = line;
        }
    }//ifstream automatically closes when out of scope
    {
        ofstream newShop(shopFile);//override existing shop file
        for (int i = 0; i < shopFileLength; i++)//dump the array into the new shop file
            newShop << lines[i] << "\n";
    }//ofstream automatically closes when out of scope
    //-l
    expandShop();
}

void recordEnemies() {
    //-k
    ifstream enemies(enemyFile);
    string line, enemyString, enemyStats;
    int difficultyCounter = -1, enemyCounter = 0;

    while (getline(enemies, line)) {
        if (line.rfind("//", 0) == 0) continue; //skip comments

        if (line.find("#difficulty") != string::npos) {
            difficultyCounter++; //increases the difficulty of the enemies when #difficulty is found in enemies.txt
            enemyCounter = 0;
            continue;
        }
        //determines if the enemy is a boss fight
        bool isBoss = false;
        if (line.rfind("#B ", 0) == 0) {
            line.erase(0, 3);
            isBoss = true;
        }
        enemyString = line;
        getline(enemies, enemyStats);
        enemyString += "\n" + enemyStats;
        int index = enemyCounter;
        if (isBoss) {
            index = NUM_ALL_ENEMIES - 1;
        }
        switch (difficultyCounter) {
            case 0:
                easyEnemies[index] = enemyString;
                break;
            case 1:
                mediumEnemies[index] = enemyString;
                break;
            case 2:
                hardEnemies[index] = enemyString;
                break;
                //-l
            default:
                cerr << "switch case out of range\n";
        }
        //-k
        if (!isBoss) enemyCounter++;
    }
}

//-l
bool enterDungeon() {//what happens in the dungeon
    //-k
    currentHealth = maxHealth;
    cout << "Welcome to the dungeon! Here you can fight enemies who can drop money and possibly extra supplies.\n"
            "You will fight three enemies, and then a boss fight for your rewards.\nYou will not be able to access the shop while in the dungeon\n"
            "Be careful! You only have " << maxHealth
         << " HP. Once it is depleted, it's game over!\nYou will not be able to leave once you enter.\n"
            "Do you want to enter? (y/n): "; //text whenever the player enters the dungeon
    string userEnter;
    getline(cin, userEnter);
    if (tolower(userEnter[0]) == 'n') {
        cout << "exiting the dungeon\n";
        return true;
    }
    if (tolower(userEnter[0]) != 'y') {
        cout << "improper input";
        return true;
    }
    for (int i = 0; i < NUM_ALL_ENEMIES; i++) {
        string currentEnemy;
        //sets the current enemy to whatever index easy/medium/hard difficulty is at
        switch (currentDifficulty) {
            case 0: //easy difficulty
                currentEnemy = easyEnemies[i];
                break;
            case 1: //medium difficulty
                currentEnemy = mediumEnemies[i];
                break;
            case 2: //hard difficulty
                currentEnemy = hardEnemies[i];
                break;
                //-l
            default:
                cerr << "switch case out of range" << endl;
        }
        //-k
        if (!enterBattle(currentEnemy, currentDifficulty, i == NUM_ALL_ENEMIES - 1))
            return false; //check if the player died or exited during battle
    }
    currentDifficulty++;
    if (currentDifficulty > 2) {
        cout << "Congratulations! You have beaten the game!\nWe know it was short but we hope you enjoyed it.\n"
                "Type 'yay!' to end the game\n"; //victory text
        string endGame;
        while (endGame != "yay!")
            getline(cin, endGame);
        return false;
    }
    return true;
}

bool enterBattle(const string &enemyString, int enemyDifficulty, bool isBoss) {
    string name, userInput;
    int enemyHealth, enemyDamage, enemyDefence, coins, percentChance, enemyMaxHealth, totalDamage, battleStrength = 0;
    stringstream enemy(enemyString);
    getline(enemy, name);
    enemy >> enemyHealth >> enemyDamage >> enemyDefence >> coins >> percentChance;
    enemyMaxHealth = enemyHealth;
    //battle text
    if (isBoss) {
        cout << "You face the boss of this dungeon, ";
    } else {
        cout << "\nYou come across a ";
    }
    cout << name << ". What will you do?\n"
                    "- Type 'att' to attack\n"
                    "- Type 'use' to use a potion\n";

    while (enemyHealth > 0 && currentHealth > 0) { //battle loop
        cout << "You: " << currentHealth << " / " << maxHealth << "\n" << name << ": " << enemyHealth << " / "
             << enemyMaxHealth << "\n";
        //player's turn
        while (true) {
            getline(cin, userInput);
            if (userInput == "exit") return false; //ends game
            if (userInput == "att") { //attacks enemy
                totalDamage = max(damage + battleStrength - (rand() % (enemyDefence + 1)), 0);
                enemyHealth -= totalDamage;
                cout << "You dealt " << totalDamage << " damage to " << name << "\n";
            } else if (userInput == "use") { //allows the player to use one of their potions if they have any
                if (smallHealthPotionCount + mediumHealthPotionCount + largeHealthPotionCount +
                    smallStrengthPotionCount + largeStrengthPotionCount <= 0) {
                    cout << "You have no potions to use\n";
                    continue;
                }
                cout << "What would you like to use?\n";
                if (smallHealthPotionCount > 0)
                    cout << "1 - Small Healing Potion (" << smallHealthPotionCount << ")\n";
                if (mediumHealthPotionCount > 0)
                    cout << "2 - Medium Healing Potion (" << mediumHealthPotionCount << ")\n";
                if (largeHealthPotionCount > 0)
                    cout << "3 - Large Healing Potion (" << largeHealthPotionCount << ")\n";
                if (smallStrengthPotionCount > 0)
                    cout << "4 - Small Strength Potion (" << smallStrengthPotionCount << ")\n";
                if (largeStrengthPotionCount > 0)
                    cout << "5 - Large Strength Potion (" << largeStrengthPotionCount << ")\n";
                while (true) {
                    getline(cin, userInput);
                    if (userInput == "1") { //uses small healing potion
                        currentHealth += SMALL_HEALING;
                        if (currentHealth > maxHealth)
                            currentHealth = maxHealth;
                        smallHealthPotionCount--;
                        cout << " You used a Small Healing Potion and recovered " << SMALL_HEALING << " HP\n";
                    } else if (userInput == "2") { //uses medium healing potion
                        currentHealth += MEDIUM_HEALING;
                        if (currentHealth > maxHealth)
                            currentHealth = maxHealth;
                        mediumHealthPotionCount--;
                        cout << " You used a Medium Healing Potion and recovered " << MEDIUM_HEALING << " HP\n";
                    } else if (userInput == "3") { //uses large healing potion
                        currentHealth += LARGE_HEALING;
                        if (currentHealth > maxHealth)
                            currentHealth = maxHealth;
                        largeHealthPotionCount--;
                        cout << " You used a Large Healing Potion and recovered " << LARGE_HEALING << " HP\n";
                    } else if (userInput == "4") { //uses small strength potion
                        battleStrength += SMALL_STRENGTH;
                        smallStrengthPotionCount--;
                        cout << " You used a Small Strength Potion and increased your strength by " << SMALL_STRENGTH
                             << "\n";
                    } else if (userInput == "5") { //uses large strength potion
                        battleStrength += LARGE_STRENGTH;
                        largeStrengthPotionCount--;
                        cout << " You used a Large Strength Potion and increased your strength by " << LARGE_STRENGTH
                             << "\n";
                    } else {
                        cout << "Invalid input\n";
                        continue;
                    }
                    break;
                }
            } else {
                cout << "Invalid input\n";
                continue;
            }
            break;
        }
        if (enemyHealth <= 0) break;
        //enemy's turn
        totalDamage = max(enemyDamage - (rand() % (defense + 1)), 0);
        currentHealth -= totalDamage;
        cout << name << " dealt " << totalDamage << " damage to you\n";
    }
    //enemy defeat text
    if (enemyHealth <= 0) {
        if (isBoss)
            cout << "You defeated the boss, " << name
                 << ", and completed the dungeon!\nNew items are now available in the shop";
        else
            cout << "You defeated the " << name;
        cout << "\nYou have received " << coins << " coins.\n";
        money += coins; //adds the rewards after beating an enemy
        char currentPercent = rand() % 100; //random chance of getting a healing potion from defeating an enemy; bosses are guaranteed
        if (currentPercent < percentChance) {
            string potionSize;
            switch (enemyDifficulty) {
                case 0:
                    potionSize = "S";
                    smallHealthPotionCount++;
                    break;
                case 1:
                    potionSize = "M";
                    mediumHealthPotionCount++;
                    break;
                case 2:
                    potionSize = "L";
                    largeHealthPotionCount++;
                    break;
            }
            cout << "You got a Health Potion (" << potionSize << ")!\n";
        }
        cout << "\n";
        return true;
    }
    cout << "You died!\nGame Over\n"; //death message
    return false;
}