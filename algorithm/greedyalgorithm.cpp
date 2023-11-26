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

void GreedyAlgorithm::start()
{
    std::sort(items.begin(), items.end(), byArea());
    for (auto &item : items)
    {
        std::cout << item.number << " "<< item.dimentions.first << " " << item.dimentions.second << std::endl;
    }

    const auto downLimit = getDownLimit(items, binDimentions);

    for (int binNumber = 0; binNumber < downLimit; binNumber++)
    {
        bins.push_back(Bin{.dimentions = binDimentions, .number = binNumber + 1, .matrix = Matrix(binDimentions)});
    }

    bool isItemToPack = true;
    while (isItemToPack)
    {
        int numberOfPackedItems = 0;

        for (auto &item : items)
        {
            if (numberOfPackedItems >= items.size())
            {
                isItemToPack = false;
                continue;
            }
            if (item.isPacked)
            {
                numberOfPackedItems++;
                continue;
            }

            for (auto &bin : bins)
            {
                if (tryToPack(item.number, bin.number))
                {
                    numberOfPackedItems++;
                    break;
                }
            }
        }
        if (numberOfPackedItems >= items.size())
        {
            isItemToPack = false;
        }
        else
        {
            auto lastBin = bins.at(bins.size() - 1);
            bins.push_back(Bin{.dimentions = binDimentions, .number = lastBin.number + 1, .matrix = Matrix(binDimentions)});
        }
    }
}

// from elements.bin import Bin
// from elements.item import Item
// import algorithms.utils as utils

// def try_to_pack(item, bin, bin_number):
//     free_places = bin.matrix.find_free_placeholders(item.width, item.height)
//     bin.matrix.calculate_free_edges()

//     if len(free_places) == 0:
//         return False

//     [i, j] = free_places[0]

//     bin.matrix.put_item(i, j, item.width, item.height, item.number)

//     item.x = i
//     item.y = j
//     item.bin_number = bin_number
//     item.is_packed = True

//     return True

// def greedyAlgorithm(items, bin_width, bin_height):
//     # Firstly we should sort the items
//     items.sort(key=lambda x: (x.area, x.width, x.height), reverse=True)

//     [area, down_limit] = utils.get_down_limit(items, bin_width, bin_height)

//     bins = []
//     for number in range(0, down_limit):
//         bin = Bin(bin_width, bin_height, number)
//         bins.append(bin)

//     still_not_every_item_packed = True

//     while still_not_every_item_packed:
//         number_of_packed_items = 0

//         for item in items:
//             if (item.is_packed):
//                 number_of_packed_items = number_of_packed_items + 1
//                 continue

//             for i in range(0, len(bins)):
//                 if try_to_pack(item, bins[i], i):
//                     number_of_packed_items = number_of_packed_items + 1
//                     break

//         if number_of_packed_items == len(items):
//             still_not_every_item_packed = False
//         else:
//             bin = Bin(bin_width, bin_height, down_limit+1)
//             bins.append(bin)

//     for item in items:
//         print(item.width, item.height, item.x, item.y, item.bin_number)

//     return items, len(bins)
