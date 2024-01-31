#include "precisealgorithm.h"

#include <iostream>
#include <thread>

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

void PreciseAlgorithm::addItems(std::vector<Item> &packedItems) {
    std::lock_guard<std::mutex> guard(mutex);
    for (auto &item : packedItems){
        items.push_back(item);
    }
    std::cout << "-----------------------\n Current packed items:" << std::endl;
    for (auto &item : packedItems){
        std::cout << item.number << std::endl;
    }

    conditionVariable.notify_all(); // Notify waiting threads about the data change
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
        std::cout << "Bin "<< bin.number << " has been finished with best value: " <<  bestValue << std::endl;
        return bestValue;
    }

    auto [lowestEdgeValue, itemHasBeenPacked] = addItem(item, bin);
    if (!itemHasBeenPacked){
        return int(lowestEdgeValue);
    }

    bestValue = std::min(bestValue, lowestEdgeValue);

    packedItems.push_back(item);
    itemsForBin.erase(std::remove_if(itemsForBin.begin(), itemsForBin.end(), [&item](auto &elem){return elem.number == item.number;}));


    if (itemsForBin.empty()){
        std::cout << "Bin "<< bin.number << " has been finished with best value: " <<  bestValue << std::endl;
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

void PreciseAlgorithm::startPacking(std::vector<Item> packedItems, Bin bin, Item item, std::vector<Item> itemsForBin, int bestValue, int depth) {
    packItem(packedItems, bin, item, itemsForBin, bestValue, depth);
    addItems(packedItems);
}

void PreciseAlgorithm::startWithMultithreads()
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

    std::vector<std::thread> threads;
    for (int i = 0; i < itemsForBins.size(); i++){
        std::vector<Item> packedItems;
        auto bin = bins[i];
        auto item = itemsForBins[i][0];
        int bestValue = WORST_VALUE;
        int depth = 0;

        std::thread thread(&PreciseAlgorithm::startPacking, this, packedItems, bin, item, itemsForBins[i], bestValue, depth);
        threads.push_back(std::move(thread));
    }

    for (auto &thread: threads){
        thread.join();
    }
}

int PreciseAlgorithm::packItemVectorization(std::vector<Item> &packedItems, Bin &bin, Item &item, std::vector<Item> &itemsForBin, int &bestValue, int &depth){
    if (itemsForBin.empty()){
        std::cout << "Bin "<< bin.number << " has been finished with best value: " <<  bestValue << std::endl;
        return bestValue;
    }

    auto [lowestEdgeValue, itemHasBeenPacked] = addItem(item, bin);
    if (!itemHasBeenPacked){
        return int(lowestEdgeValue);
    }

    bestValue = std::min(bestValue, lowestEdgeValue);

    packedItems.push_back(item);
    itemsForBin.erase(std::remove_if(itemsForBin.begin(), itemsForBin.end(), [&item](auto &elem){return elem.number == item.number;}));


    if (itemsForBin.empty()){
        std::cout << "Bin "<< bin.number << " has been finished with best value: " <<  bestValue << std::endl;
        return bestValue;
    }

    #pragma omp parallel for simd
    for (auto &nextItem: itemsForBin){
        depth++;
        auto newBestValue = packItem(packedItems, bin, nextItem, itemsForBin, bestValue, depth);
        if (itemsForBin.empty()){
            #pragma omp critical
            return newBestValue;
        }
    }

    bin.matrix.removeItem(packedItems.back());
    itemsForBin.insert(itemsForBin.begin(), Item(packedItems.back().dimentions, packedItems.back().number));
    packedItems.pop_back();
    return bestValue;
}

int checkOccupiedAreaVectorization(const std::vector<Item> &items){
//------------------------------- org
    // int maxArea = 0;
    // for (const auto &item: items){
    //     maxArea += item.getArea();
    // }

    // return maxArea;
//-------------------------------

    // std::vector<int> areas;
    // areas.reserve(items.size());

    // std::transform(items.begin(), items.end(), std::back_inserter(areas),
    //     [](const Item &item) { return item.getArea(); });

    // int maxArea = std::accumulate(areas.begin(), areas.end(), 0);

    // return maxArea;

//-------------------------------
    // Upewnij się, że ilość elementów jest podzielna przez 4 (rozmiar SSE2)
    size_t size = items.size() / 4 * 4;

    __m128i sum = _mm_setzero_si128();

    // Przetwarzaj po 4 elementy jednocześnie
    #pragma omp simd
    for (size_t i = 0; i < size; i += 4) {
        __m128i areas = _mm_set_epi32(
            items[i + 3].getArea(),
            items[i + 2].getArea(),
            items[i + 1].getArea(),
            items[i].getArea()
        );

        sum = _mm_add_epi32(sum, areas);
    }

    // Sumuj wyniki
    alignas(16) int result[4];
    _mm_store_si128(reinterpret_cast<__m128i*>(result), sum);

    int maxArea = result[0] + result[1] + result[2] + result[3];

    // Obsłuż pozostałe elementy (jeśli ich ilość nie była podzielna przez 4)
    #pragma omp simd
    for (size_t i = size; i < items.size(); ++i) {
        maxArea += items[i].getArea();
    }

    return maxArea;
}

std::vector<std::vector<Item>> divideItemsToBinsVectorization(std::vector<Item> &items, const int numberOfBins){
    std::vector<std::vector<Item>> itemsForBins(numberOfBins);

    #pragma omp simd
    for (auto &item : items) {
        int leastOccupiedBinArea = WORST_VALUE;
        int bestBin = NONE;

        #pragma omp simd
        for (int i = 0; i < itemsForBins.size(); i++){
            auto occupiedArea = checkOccupiedAreaVectorization(itemsForBins[i]);

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

int PreciseAlgorithm::getDownLimitVectorization(const std::vector<Item> &items, const std::pair<int, int> binDimentions)
{
    // We have to calculate down limit of number of items:
    const auto &[binWidth, binHeight] = binDimentions;
    const auto binArea = binWidth * binHeight;

//------------------------------- org
    // int itemsArea = 0;

    // for (const auto & item:items){
    //     itemsArea += item.getArea();
    // }

    // int downLimit = 0;

    // while (binArea * downLimit < itemsArea)
    // {
    //     downLimit++;
    // }

//-------------------------------

    std::vector<int> itemAreas;
    itemAreas.reserve(items.size());

    std::transform(items.begin(), items.end(), std::back_inserter(itemAreas),[](const Item &item) { return item.getArea(); });

    int itemsArea = std::accumulate(itemAreas.begin(), itemAreas.end(), 0);

    int downLimit = (itemsArea + binArea - 1) / binArea;

//--------------------------------
    // __m256i sumVector = _mm256_setzero_si256();

    // #pragma omp simd reduction(+:itemsArea)
    // for (const auto &item : items) {
    //     const int area = item.getArea();
    //     __m256i areaVector = _mm256_set1_epi32(area);
    //     sumVector = _mm256_add_epi32(sumVector, areaVector);
    // }

    // alignas(32) int sumResult[8];
    // _mm256_store_si256((__m256i*)sumResult, sumVector);

    // int itemsArea = 0;
    // for (int i = 0; i < 8; ++i) {
    //     itemsArea += sumResult[i];
    // }

    // int downLimit = (itemsArea + binArea - 1) / binArea;
//-------------------------------

    std::cout << "Full area of items = " << itemsArea << std::endl;
    std::cout << "Full area of bins = " << binArea * downLimit << std::endl;
    std::cout << "Down limit of bins = " << downLimit << std::endl;

    return downLimit;
}

void PreciseAlgorithm::startWithVectorization()
{
    std::sort(items.begin(), items.end(), byArea());
    // for (auto &item : items) {
    //     std::cout << item.number << " "<< item.dimentions.first << " " << item.dimentions.second << std::endl;
    // }

    const auto downLimit = getDownLimitVectorization(items, binDimentions);

    #pragma omp simd
    for (int binNumber = 0; binNumber < downLimit; binNumber++) {
        bins.push_back(Bin{.dimentions = binDimentions, .number = binNumber + 1, .matrix = Matrix(binDimentions)});
    }

    auto itemsForBins =  divideItemsToBinsVectorization(items, downLimit);
    items.clear();

    int currentBin = 0;
    #pragma omp simd
    for (auto &itemsForBin: itemsForBins){
        std::vector<Item> packedItems;
        auto &bin = bins[currentBin];
        auto &item = itemsForBin[0];
        int bestValue = WORST_VALUE;
        int depth = 0;
        packItemVectorization(packedItems, bin, item, itemsForBin, bestValue, depth);

        #pragma omp simd
        for (auto &item: packedItems){
            items.push_back(item);
        }
        currentBin++;
    }
}