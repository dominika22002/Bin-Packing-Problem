#pragma once

#include <utility>

constexpr int NONE = -1;

class Item
{
public:
    std::pair<int, int> coordinates = {NONE, NONE};
    std::pair<int, int> dimentions = {NONE, NONE};
    int number = NONE;
    int binNumber = NONE;
    bool isPacked = false;
    bool isRotated = false;

    Item(const std::pair<int, int> &newDimentions, const int newNumber);
    Item(const std::pair<int, int> &newCoordinates, const std::pair<int, int> &newDimentions, const int newNumber);

    int getArea() const;
    void rotate();
};

struct byArea
{
    inline bool operator()(Item &item1, Item &item2)
    {
        return (item1.getArea() > item2.getArea());
    }
};
