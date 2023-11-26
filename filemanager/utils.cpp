#include "utils.h"

#include <random>

namespace {

// Range of bin
constexpr int MAX_BIN_SIZE = 40;
constexpr int MIN_BIN_SIZE = int(MAX_BIN_SIZE/2);

// Range of number of bins
constexpr int MAX_BIN_NUMBER = 5;
constexpr int MIN_BIN_NUMBER = 5;

// Minimal size of item
constexpr int MIN_ITEM_SIZE = 3;

std::pair<int,int> generateBin(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(MIN_BIN_SIZE,MAX_BIN_SIZE);

    const auto dimention1 = distr(gen);
    const auto dimention2 = distr(gen);

    return {std::min(dimention1,dimention2),std::max(dimention1,dimention2) };
}

std::vector<Item> generateItems(const int binWidth, const int binHeight, const int numberOfBins){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(MIN_ITEM_SIZE, int(std::min(binWidth, binHeight)/2));

    const auto dimention1 = distr(gen);

    const int binArea = binWidth * binHeight;
    const int maxBinArea = binWidth * binHeight * numberOfBins;
    const int minItemsArea = maxBinArea*0.95;
    const int maxItemsArea = maxBinArea;

    bool isItemToGenerate = true;
    int currentItemsArea = 0;
    int itemNumber = 0;
    std::vector<Item> items;

    while (isItemToGenerate){
        const auto dimention1 = distr(gen);
        const auto dimention2 = distr(gen);

        const auto width = std::min(dimention1, dimention2);
        const auto height = std::max(dimention1, dimention2);

        const int itemArea = width * height;

        if (itemArea + currentItemsArea <= maxItemsArea){
            itemNumber++;
            items.push_back(Item({width, height},itemNumber));
            currentItemsArea += itemArea;
        }

        if ( currentItemsArea >= minItemsArea){
            isItemToGenerate = false;
        }
    }
    return items;
}

int generateBinsNumber(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(MAX_BIN_NUMBER, MIN_BIN_NUMBER);

    return distr(gen);
}

}


int Utils::getNumberOfTestFiles(){
    bool  isFileExist = true;
    int  currentNumber = 0;
    while(isFileExist){
        currentNumber++;
        const auto fileName = FILE_PATH + std::to_string(currentNumber) + ".txt";
        if (!std::filesystem::exists(fileName)){
            isFileExist = false;
        }
    }
    return currentNumber - 1;
}

void Utils::generate() {
    const auto [binWidth, binHeight] = generateBin();
    const auto items = generateItems(binWidth, binHeight, generateBinsNumber());

    const int newTestFileNumber = getNumberOfTestFiles()+1;

    std::ofstream newTestFile;
    newTestFile.open(Utils::FILE_PATH + std::to_string(newTestFileNumber) + ".txt");

    if (newTestFile.is_open()) {
        newTestFile << binWidth << "\t" << binHeight << "\n";

        for (const auto &item : items){
            const auto &[width,height] = item.dimentions;
            newTestFile << item.number << "\t";
            newTestFile << width << "\t";
            newTestFile << height << "\t";
            newTestFile << "\n";
        }
        newTestFile.close();
    }
}
