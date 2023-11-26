#include "precisealgorithm.h"

#include <iostream>

namespace {

constexpr int WORST_VALUE = 100000000;

int getDownLimit(const std::vector<Item> &items, const std::pair<int, int> binDimentions)
{
    // We have to calculate down limit of number of items:
    const auto &[binWidth, binHeight] = binDimentions;
    const auto binArea = binWidth * binHeight;
    int itemsArea = 0;

    for (const auto & item:items){
        itemsArea += item.getArea();
    }

    int downLimit = 0;

    while (binArea * downLimit < itemsArea)
    {
        downLimit++;
    }

    std::cout << "Full area of items = " << itemsArea << std::endl;
    std::cout << "Full area of bins = " << binArea * downLimit << std::endl;
    std::cout << "Down limit of bins = " << downLimit << std::endl;

    return downLimit;
}

int checkOccupiedArea(const std::vector<Item> &items){
    int maxArea = 0;
    for (const auto &item: items){
        maxArea += item.getArea();
    }

    return maxArea;
}

void showItems(const std::vector<Item> &items, std::string title){
    std::cout << title << "\n" << std::endl;
    for (const auto &item: items){
        std::cout << item.number << std::endl;
    }
}

std::vector<std::vector<Item>> divideItemsToBins(std::vector<Item> &items, const int numberOfBins){
    std::vector<std::vector<Item>> itemsForBins(numberOfBins);

    for (auto &item : items) {
        int leastOccupiedBinArea = WORST_VALUE;
        int bestBin = NONE;

        for (int i = 0; i < itemsForBins.size(); i++){
            auto occupiedArea = checkOccupiedArea(itemsForBins[i]);

            if (occupiedArea< leastOccupiedBinArea){
                leastOccupiedBinArea = occupiedArea;
                bestBin = i;
            }
        }

        item.binNumber = bestBin;
        itemsForBins[bestBin].push_back(item);
    }
    return itemsForBins;
}

}

PreciseAlgorithm::PreciseAlgorithm(const std::vector<Item> &newItems, const std::pair<int, int> newBinDimentions) : items(newItems), bins({}), binDimentions(newBinDimentions)
{
    std::cout << "PreciseAlgorithm ready to work!\n";

}

const std::vector<Item> &PreciseAlgorithm::getItems() const {
    return items;
}

const std::vector<Bin> &PreciseAlgorithm::getBins() const{
    return bins;
}

bool PreciseAlgorithm::tryToPack(const int itemNumber, const int binNumber)
{
    Item *itemToPack;
    Bin *binToPack;
    for (auto &item : items)
    {
        if (item.number == itemNumber)
        {
            itemToPack = &item;
        }
    }
    for (auto &bin : bins)
    {
        if (bin.number == binNumber)
        {
            binToPack = &bin;
        }
    }
    auto freePlaces = binToPack->matrix.findFirstFreePlaceholder(*itemToPack);

    if (!freePlaces.has_value()){
        return false;
    }

    binToPack->matrix.caluclateFreeEdges();

    itemToPack->coordinates = freePlaces.value();
    binToPack->matrix.putItem(*itemToPack);

    // binToPack->matrix.show();

    itemToPack->binNumber = binToPack->number;
    itemToPack->isPacked = true;

    return itemToPack->isPacked;
}

std::pair<std::pair<int,int> ,int>  PreciseAlgorithm::findBestCoordinates(Item &item, Bin &bin){
    const auto freePlaces = bin.matrix.findFreePlaceholders(item);
    int lowestEdgeValue = WORST_VALUE;
    std::pair<int,int> bestCoordinates = {NONE, NONE};

    for (auto &[i, j] : freePlaces){
        bin.matrix.putItem(Item({i, j}, item.dimentions, item.number));
        auto freeEdgesValue = bin.matrix.caluclateFreeEdges();

        if (lowestEdgeValue > freeEdgesValue){
            lowestEdgeValue = freeEdgesValue;
            bestCoordinates = {i, j};
        }

        bin.matrix.removeItem(item);
    }
    return {bestCoordinates, lowestEdgeValue};
}

std::pair<int,bool> PreciseAlgorithm::addItem(Item &item, Bin &bin){
    auto [bestCoordinates, lowestEdgeValue] = findBestCoordinates(item,bin);
    item.rotate();
    auto [bestCoordinatesRotate, lowestEdgeValueRotate] = findBestCoordinates(item,bin);
    item.rotate();

    if (lowestEdgeValue == WORST_VALUE && lowestEdgeValueRotate == WORST_VALUE){
        return {WORST_VALUE, false};
    }

    if (lowestEdgeValue > lowestEdgeValueRotate){
        item.rotate();
        bestCoordinates = bestCoordinatesRotate;
    }

    bin.matrix.putItem(Item(bestCoordinates, item.dimentions, item.number));
    item.coordinates = bestCoordinates;
    item.binNumber = bin.number;
    item.isPacked = true;

    return {std::min(lowestEdgeValue, lowestEdgeValueRotate), true};
}

int PreciseAlgorithm::packItem(std::vector<Item> &packedItems, Bin &bin, Item &item, std::vector<Item> &itemsForBin, int &bestValue, int &depth){
    if (itemsForBin.empty()){
        std::cout << bestValue << std::endl;
        int bestValue;
    }

    auto [lowestEdgeValue, itemHasBeenPacked] = addItem(item, bin);
    if (!itemHasBeenPacked){
        return int(lowestEdgeValue);
    }

    bestValue = std::min(bestValue, lowestEdgeValue);

    packedItems.push_back(item);
    itemsForBin.erase(std::remove_if(itemsForBin.begin(), itemsForBin.end(), [&item](auto &elem){return elem.number == item.number;}));


    if (itemsForBin.empty()){
        std::cout << bestValue << std::endl;
        return bestValue;
    }

    for (auto &nextItem: itemsForBin){
        depth++;
        auto newBestValue = packItem(packedItems, bin, nextItem, itemsForBin, bestValue, depth);
        if (itemsForBin.empty()){
            return newBestValue;
        }
    }

    bin.matrix.removeItem(packedItems.back());
    itemsForBin.insert(itemsForBin.begin(), Item(packedItems.back().dimentions, packedItems.back().number));
    packedItems.pop_back();
    return bestValue;
}

void PreciseAlgorithm::start()
{
    std::sort(items.begin(), items.end(), byArea());
    for (auto &item : items) {
        std::cout << item.number << " "<< item.dimentions.first << " " << item.dimentions.second << std::endl;
    }

    const auto downLimit = getDownLimit(items, binDimentions);

    for (int binNumber = 0; binNumber < downLimit; binNumber++) {
        bins.push_back(Bin{.dimentions = binDimentions, .number = binNumber + 1, .matrix = Matrix(binDimentions)});
    }

    auto itemsForBins =  divideItemsToBins(items, downLimit);
    items.clear();

    int currentBin = 0;
    for (auto &itemsForBin: itemsForBins){
        std::vector<Item> packedItems;
        auto &bin = bins[currentBin];
        auto &item = itemsForBin[0];
        int bestValue = WORST_VALUE;
        int depth = 0;
        packItem(packedItems, bin, item, itemsForBin, bestValue, depth);

        for (auto &item: packedItems){
            items.push_back(item);
        }
        currentBin++;
    }
}
