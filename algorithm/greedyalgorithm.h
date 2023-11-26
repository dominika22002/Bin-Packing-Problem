#pragma once
#include "../bin.h"

#include <iostream>

class GreedyAlgorithm final
{
    std::vector<Item> items;
    std::vector<Bin> bins;
    std::pair<int, int> binDimentions;

    bool tryToPack(const int itemNumber, const int binNumber);

public:
    void start();
    const std::vector<Item> &getItems() const ;
    const std::vector<Bin> &getBins() const ;

    GreedyAlgorithm(const std::vector<Item> &newItems, const std::pair<int, int> newBinDimentions);
};
