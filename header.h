#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <set>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;
//functions made by -l:
void makeShop();
void removeShop();
bool addMoney(const string& input);
void playerAction(const string& input);
unsigned int getShopLength();
void openShop();
void buyItem();
void expandShop();
void removeShopItem(const string& itemName);
void enterDungeon();
void recordEnemies();

//functions made by -k:
void startingText();
void playerStats();
bool enterBattle(const string &enemyString, int enemyDifficulty);