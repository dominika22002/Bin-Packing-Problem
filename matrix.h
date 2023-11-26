#pragma once
#include <vector>
#include <string>
#include <optional>
#include <iostream>

#include "item.h"

namespace {
    bool isProperDimentions(const Item &item, const int rows, const int columns){
        const auto &[x, y] = item.coordinates;
        const auto &[width, height] = item.dimentions;
        return (0 <= width + x && width + x <= rows && 0 <= height + y && height + y <= columns);
    }

    enum CheckProperty{
        LEFT, TOP, RIGHT, BOTTOM
    };
}

class Matrix{
    std::vector<std::vector<int>> matrix;
    int rows;
    int columns;

public:
    Matrix(const std::pair<int, int> &newDimentions);

    void show();
    bool isPossibleToPutItem(const std::pair<int,int> &coordinates, const Item &item);
    bool isItemTouchSomething(const std::pair<int,int> &coordinates, const Item &item);
    bool isNeighbour(const std::pair<int,int> &coordinates, const std::optional<CheckProperty> checkProperty = std::nullopt);
    void putItem(const Item &item);
    void removeItem(const Item &item);
    std::vector<std::pair<int,int>> findFreePlaceholders( Item item);
    std::optional<std::pair<int,int>> findFirstFreePlaceholder( const Item &item );
    int caluclateFreeEdges();
};

// class AreaMatrix:
    // def calculate_free_edges(self):
    //     free_edges = 0
    //     free_edges_list = []
    //     for i in range(self.rows):
    //         for j in range(self.columns):
    //             if self.matrix[i][j] == '-':
    //                 if self.check_index(i,j):
    //                     free_edges+=1
    //                     free_edges_list.append([i,j])
                        
    //     # self.print_free_edges(free_edges_list)
    //     return free_edges
    
    // def get_neibourhood(self, x, y):
    //     possible_items_to_remove = []
    //     if 1 <= x:
    //         if self.matrix[x-1][y] != '-':
    //             possible_items_to_remove.append(self.matrix[x-1][y])
    //     if x < self.rows-1:
    //         if self.matrix[x+1][y] != '-':
    //             possible_items_to_remove.append( self.matrix[x+1][y])
    //     if 1 <= y:
    //         if self.matrix[x][y-1] != '-':
    //             possible_items_to_remove.append(self.matrix[x][y-1])
    //     if y < self.columns-1:
    //         if self.matrix[x][y+1] != '-':
    //             possible_items_to_remove.append(self.matrix[x][y+1])

    //     return possible_items_to_remove

    // def print_free_edges(self, free_edges_list):

    //     for [i,j] in free_edges_list:
    //         self.matrix[i][j] = '+'
    //     print("\n")
    //     line = ""
    //     for i in range(self.rows):
    //         for j in range(self.columns):
    //             if self.matrix[i][j] == '-':
    //                 line += '-'
    //             elif self.matrix[i][j] == '+':
    //                 line += '+'
    //             elif int(self.matrix[i][j]) <= 9:
    //                 line += '_'
    //             line += str(self.matrix[i][j]) +" "
    //         line += '\n'
    //     print(line)
    //     for [i,j] in free_edges_list:
    //         self.matrix[i][j] = '-'
        
    // def get_items_to_remove(self):
    //     items_to_stay = []
    //     possible_items_to_stay = []
    //     items_to_remove = []
    //     possible_items_to_remove = []
    //     for i in range(self.rows):
    //         has_gap_in_row = False
    //         for j in range(self.columns):
    //             if self.matrix[i][j] != '-':
    //                 possible_items_to_stay.append(self.matrix[i][j])
    //             if self.matrix[i][j] == '-':
    //                 has_gap_in_row = True
    //                 items = self.get_neibourhood(i,j)

    //                 for item in items:
    //                     if item not in possible_items_to_remove:
    //                         possible_items_to_remove.append(item)

    //         if not has_gap_in_row:
    //             for item in possible_items_to_stay:
    //                 items_to_stay.append(item)

    //         for item in possible_items_to_remove:
    //             if item not in items_to_remove:
    //                 items_to_remove.append(item)
            
    //     return items_to_remove
