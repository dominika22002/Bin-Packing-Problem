#include "greedyalgorithm.h"

namespace {

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

}

GreedyAlgorithm::GreedyAlgorithm(const std::vector<Item> &newItems, const std::pair<int, int> newBinDimentions) : items(newItems), bins({}), binDimentions(newBinDimentions)
{
    std::cout << "GreedyAlgorithm ready to work!\n";

} //: Algorithm(newItems){}

const std::vector<Item> &GreedyAlgorithm::getItems() const {
    return items;
}

const std::vector<Bin> &GreedyAlgorithm::getBins() const{
    return bins;
}

bool GreedyAlgorithm::tryToPack(const int itemNumber, const int binNumber)
{
    Item *itemToPack;
    Bin *binToPack;
    for (auto &item : items) {
        if (item.number == itemNumber) {
            itemToPack = &item;
        }
    }
    for (auto &bin : bins) {
        if (bin.number == binNumber) {
            binToPack = &bin;
        }
    }
    auto freePlaces = binToPack->matrix.findFirstFreePlaceholder(*itemToPack);

    if (!freePlaces.has_value()) {
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

void GreedyAlgorithm::start()
{
    std::sort(items.begin(), items.end(), byArea());
    for (auto &item : items) {
        std::cout << item.number << " "<< item.dimentions.first << " " << item.dimentions.second << std::endl;
    }

    const auto downLimit = getDownLimit(items, binDimentions);

    for (int binNumber = 0; binNumber < downLimit; binNumber++)
    {
        bins.push_back(Bin{.dimentions = binDimentions, .number = binNumber + 1, .matrix = Matrix(binDimentions)});
    }

    bool isItemToPack = true;
    while (isItemToPack) {
        int numberOfPackedItems = 0;

        for (auto &item : items) {
            if (numberOfPackedItems >= items.size()) {
                isItemToPack = false;
                continue;
            }
            if (item.isPacked) {
                numberOfPackedItems++;
                continue;
            }

            for (auto &bin : bins) {
                if (tryToPack(item.number, bin.number))
                {
                    numberOfPackedItems++;
                    break;
                }
            }
        }
        if (numberOfPackedItems >= items.size()) {
            isItemToPack = false;
        } else {
            auto lastBin = bins.at(bins.size() - 1);
            bins.push_back(Bin{.dimentions = binDimentions, .number = lastBin.number + 1, .matrix = Matrix(binDimentions)});
        }
    }
}
