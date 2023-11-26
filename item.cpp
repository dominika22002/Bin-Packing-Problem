#include "item.h"

int Item::getArea() const
{
    const auto &[width, height] = dimentions;
    return width * height;
}

void Item::rotate()
{
    auto &[width, height] = dimentions;
    std::swap(width, height);

    isRotated = !isRotated;
}

Item::Item(const std::pair<int, int> &newDimentions, const int newNumber) : dimentions(newDimentions), number(newNumber)
{
}

Item::Item(const std::pair<int, int> &newCoordinates, const std::pair<int, int> &newDimentions, const int newNumber) : coordinates(newCoordinates), dimentions(newDimentions), number(newNumber)
{
}
