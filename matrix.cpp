#include "matrix.h"

Matrix::Matrix(const std::pair<int, int> &newDimentions) : rows(newDimentions.first), columns(newDimentions.second){
    for (int i = 0; i < rows; i++)
    {
        std::vector<int> row;
        for (int j = 0; j < columns; j++)
        {
            row.push_back(NONE);
        }
        matrix.push_back(row);
    }

}

void Matrix::show(){
    std::string text = "";
    for (const auto&row: matrix){
        for (const auto &element: row){
            text += std::to_string(element);
            text += " ";
        }
        text += "\n";
    }
    std::cout << text << std::endl;
}

bool Matrix::isPossibleToPutItem(const std::pair<int, int> &coordinates, const Item &item){
    const auto &[x, y] = coordinates;
    const auto &[width, height] = item.dimentions;

    if (!isProperDimentions(Item(coordinates,item.dimentions,item.number), rows, columns)){
        return false;
    }

    for (int i = x; i < width+x; i++){
        for (int j = y; j < height+y; j++){
            if (matrix[i][j] !=NONE){
                return false;
            }
        }
    }
    return true;
}

void Matrix::putItem(const Item &item){
    const auto &[x, y] = item.coordinates;
    const auto &[width, height] = item.dimentions;

    for (int i = x; i < width+ x; i++){
        for (int j = y; j < height+ y; j++){
            matrix[i][j] = item.number;
        }
    }
}

void Matrix::removeItem(const Item &item){
    for (auto &row: matrix){
        for (auto &element: row){
            if (element == item.number){
                element = NONE;
            }
        }
    }
}

std::vector<std::pair<int, int> > Matrix::findFreePlaceholders(Item item){
    std::vector<std::pair<int,int>> possibleCoordinates;

    for (int i = 0; i < rows; i++ ) {
        for (int j = 0; j < columns; j++ ) {
            if (matrix[i][j] == NONE && isPossibleToPutItem({i,j}, item) && isItemTouchSomething({i,j},item)){
                possibleCoordinates.push_back({i,j});
            }
        }
    }

    return possibleCoordinates;
}

std::optional<std::pair<int, int> > Matrix::findFirstFreePlaceholder(const Item &item){
    for (int i = 0; i < rows; i++ ) {
        for (int j = 0; j < columns; j++ ) {
            if (matrix[i][j] == NONE && isPossibleToPutItem({i,j}, item) && isItemTouchSomething({i,j},item)){
                return std::pair<int,int>{i,j};
            }
        }
    }

    return std::nullopt;
}

bool Matrix::isItemTouchSomething(const std::pair<int, int> &coordinates, const Item &item){
    const auto itemToCheck = Item{coordinates, item.dimentions, item.number};
    putItem(itemToCheck);

    const auto &[x, y] = coordinates;
    const auto &[width, height] = item.dimentions;

    auto isItemTouch = false;

    for (int i = x; i < width+ x; i++){
        if (isNeighbour({i,y},CheckProperty::TOP)){
            isItemTouch = true;
        }

        if (isNeighbour({i, y+height},CheckProperty::BOTTOM)){
            isItemTouch = true;
        }
    }
    for (int i = y; i < height+ y; i++){
        if (isNeighbour({x, i},CheckProperty::LEFT)){
            isItemTouch = true;
        }

        if (isNeighbour({x+width,i},CheckProperty::RIGHT)){
            isItemTouch = true;
        }
    }

    removeItem(itemToCheck);
    return isItemTouch;
}

bool Matrix::isNeighbour(const std::pair<int, int> &coordinates, const std::optional<CheckProperty> checkProperty){
    const auto &[x, y] = coordinates;

    if (!checkProperty.has_value()){
        return ((x == 0 || ( x >= 1 && matrix[x-1][y] != NONE)) || (y == 0 || ( y >= 1 && matrix[x][y-1] != NONE)) || (x == rows-1 || ( x < rows-1 && matrix[x+1][y] != NONE)) || ( y == columns-1 || ( y >= columns-1 && matrix[x][y+1] != NONE)));
    }

    switch (checkProperty.value())
    {
    case CheckProperty::LEFT:
        return  x == 0 || ( x >= 1 && matrix[x-1][y] != NONE);

    case CheckProperty::TOP:
        return  y == 0 || ( y >= 1 && matrix[x][y-1] != NONE);

    case CheckProperty::RIGHT:
        return  x == rows-1 || ( x < rows-1 && matrix[x+1][y] != NONE);

    case CheckProperty::BOTTOM:
        return  y == columns-1 || ( y >= columns-1 && matrix[x][y+1] != NONE);
    }
}

int Matrix::caluclateFreeEdges(){
    int numberOfNeighbours = 0;

    for (int i = 0; i < rows; i++ ) {
        for (int j = 0; j < columns; j++ ) {
            if (matrix[i][j] == NONE && isNeighbour({i,j})){
                numberOfNeighbours += 1;
            }
        }
    }

    return numberOfNeighbours;
}
