#pragma once
#include "../bin.h"
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <numeric>
#include <immintrin.h>

class PreciseAlgorithm final
{
    std::vector<Item> items;
    std::vector<Bin> bins;
    std::pair<int, int> binDimentions;

    bool tryToPack(const int itemNumber, const int binNumber);
    int packItem(std::vector<Item> &packedItems, Bin &bin, Item &item, std::vector<Item> &itemsForBin, int &bestValue, int &depth);

    std::pair<int,bool> addItem(Item &item, Bin &bin);
    std::pair<std::pair<int,int> ,int>  findBestCoordinates(Item &item, Bin &bin);

    // Multithreading:
    std::mutex mutex;
    std::condition_variable conditionVariable;
    void addItems(std::vector<Item> &packedItems);
    void startPacking(std::vector<Item> packedItems, Bin bin, Item item, std::vector<Item> itemsForBin, int bestValue, int depth);

    // Vectorization
    int packItemVectorization(std::vector<Item> &packedItems, Bin &bin, Item &item, std::vector<Item> &itemsForBin, int &bestValue, int &depth);
    int getDownLimitVectorization(const std::vector<Item> &items, const std::pair<int, int> binDimentions);

public:
    void start();
    void startWithMultithreads();
    const std::vector<Item> &getItems() const ;
    const std::vector<Bin> &getBins() const ;
    void startWithVectorization();

    PreciseAlgorithm(const std::vector<Item> &newItems, const std::pair<int, int> newBinDimentions);
};

