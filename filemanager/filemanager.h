#pragma once

#include <vector>

#include "../item.h"


namespace FileManager {

void writeToFile(const std::vector<Item> &items, const std::pair<int,int> & binDimentions);
int createNewTestFile();
std::pair<std::vector<Item>, std::pair<int,int>> getValuesFromFile(const int fileNumber);

}
