//
// Created by liamg on 10/31/2023.
//
#include "header.h"

unsigned long long int money=0;
long long int baseMoneyAdd=1; //the base amount of money you will get by pressing space
long long int moneyMultiplier=1; //multiplies base money to get total money every time you press space
int currentShopLevel=1;
const unsigned int NUMALLLOCATIONS=2;
string allLocations[NUMALLLOCATIONS]={"outside", "dungeon"};  //all locations, including currently locked locations; unknown how many locations should have
string possibleLocations[]={"outside", "dungeon"}; //all locations player can currently travel; always keep outside as first
unsigned int numAvailLocations= sizeof(possibleLocations) / sizeof(possibleLocations[0]); //current number of locations the player can travel to
string playerLocation=possibleLocations[0]; //current player location. At the beginning of the game, location is set to outside
const string shopFile="shop.txt";
const string enemyFile="enemies.txt";
const unsigned int shopFileLength=getShopLength();//gets how many lines are in the shop file check with prof which method is better


void makeShop(){//pastes the original shop into the shop file
    ifstream shopLine("baseShop.txt");
    ofstream toFile(shopFile);
    string line;

    while (getline(shopLine,line)){
        toFile<<line<<endl;
    }
    shopLine.close();
    toFile.close();
}

void removeShop(){//removes shop file before code ends so duplicate files don't appear after multiple runs
    remove(shopFile.c_str());
}


void playerAction(const string& input){
    if (addMoney(input))
        return;
//    else if (input=="location"){
//        changeLocation(input);
//        switch (playerLocation) {
//            case 0:
//                //put code to return player back to the outside area
//                break;
//        }
//    }
    else if (input=="shop"){
        openShop();
        return;
    }else if (input=="buy item"){
        buyItem();
        return;
    }
}

bool addMoney(const string& input){
    if (input.length()==1 && isspace(input[0])){
        money+=baseMoneyAdd*moneyMultiplier;
        cout<<"Total money: "<<money<<endl;
        return true;
    }
    return false;
}

bool changeLocation(const string& input){
    string newLocation;
    cout<<"Current Location: "<<playerLocation<<endl;
    cout<<"Where would you like to go? ";
    cin>>newLocation;
    cout<<endl;

    for (char i : input){ //loops through chars in the string, making all characters lowercase
        if (i>=65 && i<=90)
            i+=32;
    }
    if (input==newLocation)//if player is trying to go to a place where they already are
        return false;
    for (const string& location : possibleLocations){ //loops through the possible locations, setting the new player location to user inputted if it is a possible location to travel to
        if (location == input){
            playerLocation=location;
            cout<<"New Location: "<<playerLocation<<endl; //tells player their new location
            return true; //add statement that triggers depending on what location they are now in
        }
    }
    return false;
}

int getShopLength(){
    ifstream shopLine("baseShop.txt");
    string line;
    unsigned int counter=0;
    while (getline(shopLine,line)){
        counter++;
    }
    return  (unsigned)counter;
}

void openShop() {
    ifstream getShopItem(shopFile);
    string line, start, item, name, cost;
    int rows,columns;
    cout<<"Current Shop Level: "<<currentShopLevel<<endl;
    for (int shopCount=1; shopCount<=currentShopLevel; shopCount++){ //outputs the number of shops equal to their level
        getline(getShopItem, line);//gets shop statement
        stringstream ss(line);
        ss>>start>>rows>>columns; //separates the line into the "#shop", width and height
        for (int currentRow=0, currentCol=0; currentRow<=rows; currentCol++){ //for the size of the shop print out appropriate names and spaces in the console
            getline(getShopItem,line);//gets next shop item from file
            stringstream shopLine(line);
            shopLine>>name>>cost;//splits line into name and cost
            if (name=="#purchased"){ //if the item is purchased, leave a blank space where the item usually goes and a buffer for the next item
                cout<<setw(20);
            }else {
                cout << setw(10) <<name; //couts next item with space
            }
            if (currentCol==columns){ //if the current column you are on the max amount of columns
                currentCol=0; //reset columns
                currentRow++; //go to next row
                cout<<setw(10)<<endl<<setw(0); //end the line for the next row starts the next row indented
                if (currentRow>rows){ //if the next row is bigger than the max amount of rows in the shop, iterate the loop which causes the loop to break
                    continue;
                }
                getline(getShopItem,line);//gets the next item for column 0
                stringstream newShop(line);
                newShop>>name>>cost;
                cout<<setw(10)<<name; //couts column 0 item with space
            }
        }
    }
    getShopItem.close();
}

void buyItem(){
    ifstream getShopItem(shopFile);
    string itemToBuy,shopItem,itemName;
    int shopLevel=0,itemCost;//shop level starts at 0 b/c the first line of shop file adds one level
    bool boughtitem=false;
    cout<<"What item would you like to buy: ";
    getline(cin,itemToBuy); //gets item user would like to buy
    while (getline(getShopItem,shopItem)){ //while there is a line to get from the file
        if (shopItem.find("#shop")!=string::npos){ //if the line that is being read is a shop header, continue and add one to the shop level
            shopLevel++;
            continue;
        }else{
            stringstream ss(shopItem); //split the line into the name and the cost
            ss >> itemName >> itemCost;
            if(itemToBuy==itemName) {//if the line in the txt file contains the name the user inputted
                if (shopLevel>currentShopLevel){//if the item is in a shop the user has not unlocked yet
                    cout<<"The item you are trying to buy is in a locked shop"<<endl;
                    break;
                }
                if (itemCost <= money) { //if the user has enough money to buy the item
                    cout << "You have bought " << itemName << endl;
                    money -= itemCost; //update money
                    getShopItem.close();
                    boughtitem=true;
                    removeShopItem(itemName); //removes item from the shop
                    return;//prevents file from being closed twice
                } else {
                    cout << "You do not have enough money to buy this item" << endl;
                    break;
                }
            }
        }
    }
    getShopItem.close();
}

void removeShopItem(const string& itemName){ //get shopFileLength to go to array
    ifstream shopItem(shopFile);
    string shopArr[shopFileLength];//create an array with a length of lines in the shop file
    string line;

    for (unsigned int i=0; i<shopFileLength; i++){//gets every line from the file and puts each seperate line into an element in the array
        getline (shopItem, line);
        shopArr[i]=line;
    }
    shopItem.close();
    remove("shop.txt");//closes and removes the file before making another file with the same name

    ofstream newShop(shopFile);//creates new file with same name as the old one
    for (unsigned int i=0; i<shopFileLength; i++){
        if (shopArr[i].find(itemName)!=string::npos)
            newShop<<"#purchased"<<endl;
        else {
            newShop << shopArr[i];
            if (i != shopFileLength - 1)//prevents an empty line being added on every time this function is called
                newShop << endl;
        }
    }
    newShop.close();
} //check with prof to see which method is better

void dungeon(){//what happens in the dungeon

}

unsigned int difficultyLine(const string& difficulty){
    string line;
    unsigned int counter=0;
    ifstream fin(enemyFile);
    while(getline(fin, line)){
        counter++;
        if (line.find(difficulty)!=string::npos) {
            break;
        }
    }
    fin.close();
    return (counter);
}

void recordEnemies(){
    ifstream fin(enemyFile);
    string line, lengthType;
    unsigned int easyLength, mediumLength, hardLength, bossLength;
    unsigned int counter=0;
    for(int diffCounter=0;diffCounter<4;diffCounter++) {//gets the lengths of the arrays needed
        counter=0;
        do {
            getline(fin, line);
            if (line == "}") {
                counter++;
            }
        }while (line!="]");
        switch(diffCounter){//assigns length
            case 0:
                easyLength=counter;
                break;
            case 1:
                mediumLength=counter;
            case 2:
                hardLength=counter;
            case 3:
                bossLength=counter;
                break;
            default:
                cerr<<"Length Type out of range"<<endl;
                break;
        }
    }


    string easyEnemies[easyLength],mediumEnemies[mediumLength],hardEnemies[hardLength],BossEnemies[bossLength];

    fin.seekg(ios::beg);//seeks the file back to the beginning
    for (unsigned int i=0; i<sizeof(easyEnemies)/sizeof(easyEnemies[0]) ;i++){
        getline(fin, line);
        if (line.find('{')!=string::npos){
            line.erase(line.length()-1);//erases the bracket from the string
            easyEnemies[i]=line;
        }
    }
    for (unsigned int i=0; i<sizeof(mediumEnemies)/sizeof(mediumEnemies[0]) ;i++){
        getline(fin, line);
        if (line.find('{')!=string::npos){
            line.erase(line.length()-1);//erases the bracket from the string
            easyEnemies[i]=line;//set the value to the enemy name
            continue;
        }else{//if no value was added, do not increment
            i--;
        }
    }

    fin.close();
}



void getEnemy(const string& difficulty) {//enemy to get while in dungeon
    string  enemy, line;




}
//const unsigned int easyLineLocation= getEnemyDifficultyLine("Easy"),mediumLineLocation= difficultyLine("Medium"),
//hardLineLocation= getEnemyDifficultyLine("Hard"),bossLineLocation= difficultyLine("Boss");
