#pragma once
#include "../bin.h"

class PreciseAlgorithm final
{
    std::vector<Item> items;
    std::vector<Bin> bins;
    std::pair<int, int> binDimentions;

    bool tryToPack(const int itemNumber, const int binNumber);
    int packItem(std::vector<Item> &packedItems, Bin &bin, Item &item, std::vector<Item> &itemsForBin, int &bestValue, int &depth);

    std::pair<int,bool> addItem(Item &item, Bin &bin);
    std::pair<std::pair<int,int> ,int>  findBestCoordinates(Item &item, Bin &bin);


public:
    void start();
    const std::vector<Item> &getItems() const ;
    const std::vector<Bin> &getBins() const ;

    PreciseAlgorithm(const std::vector<Item> &newItems, const std::pair<int, int> newBinDimentions);
};










