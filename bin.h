#pragma once
#include "matrix.h"

class Bin
{
public:
    std::pair<int, int> dimentions = {NONE, NONE};
    int number = NONE;
    Matrix matrix;

    void putItem(const Item &item);
};
