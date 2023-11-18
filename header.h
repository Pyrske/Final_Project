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


void makeShop();
void removeShop();
bool addMoney(const string& input);
string findLocation(const string& input);
void playerAction(const string& input);
bool changeLocation(const string& input);
int getShopLength();
void openShop();
void buyItem();
void removeShopItem(const string& itemName);
unsigned int difficultyLine(const string& difficulty);
void dungeon();
void recordEnemies();
void getEnemy(const string &difficulty);
