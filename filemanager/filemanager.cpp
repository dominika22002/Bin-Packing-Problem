#include "filemanager.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>

#include "utils.h"

namespace {

std::pair<int,int> deserializeBin(const std::string &line){
    std::istringstream iss(line);
    std::vector<std::string> splittedString;

    while (iss) {
        std::string word;
        iss >> word;
        if (!word.empty()) {
            splittedString.push_back(word);
        }
    }

    return {std::atoi(splittedString.at(0).c_str()), std::atoi(splittedString.at(1).c_str())};
}

Item deserializeItem(const std::string &line){
    std::istringstream iss(line);
    std::vector<std::string> splittedString;

    while (iss) {
        std::string word;
        iss >> word;
        if (!word.empty()) {
            splittedString.push_back(word);
        }
    }

    const int number = std::atoi(splittedString.at(0).c_str());
    const int width = std::atoi(splittedString.at(1).c_str());
    const int height = std::atoi(splittedString.at(2).c_str());

    return Item({width,height}, number);
}

}

void FileManager::writeToFile(const std::vector<Item> &items, const std::pair<int, int> &binDimentions){
    std::ofstream outputFile;
    outputFile.open(Utils::OUTPUT_FILE);

    if (outputFile.is_open()) {
        outputFile << binDimentions.first << "\t" << binDimentions.second << "\n";

        for (const auto &item : items){
            const auto &[x,y] = item.coordinates;
            const auto &[width,height] = item.dimentions;
            outputFile << item.binNumber << "\t";
            outputFile << item.number << "\t";
            outputFile << width << "\t";
            outputFile << height << "\t";
            outputFile << x << "\t";
            outputFile << y << "\t";
            outputFile << item.isRotated << "\t";
            outputFile << "\n";
        }
        outputFile.close();
    }
    return;
}

int FileManager::createNewTestFile(){
    Utils::generate();
    return Utils::getNumberOfTestFiles();
}

std::pair<std::vector<Item>, std::pair<int, int> > FileManager::getValuesFromFile(const int fileNumber){
    const auto fileName = Utils::FILE_PATH + std::to_string(fileNumber) + ".txt";
    if (!std::filesystem::exists(fileName)){
        return {};
    }

    std::ifstream testFile(fileName); // Open a file named "example.txt" for reading
    std::vector<Item> items;
    std::pair<int,int> binDimentions;

    if (testFile.is_open()) {
        std::string line;
        std::getline(testFile, line);
        binDimentions = deserializeBin(line);

        // Read and display each line in the file
        while (std::getline(testFile, line)) {
            items.push_back(deserializeItem(line));
        }

        testFile.close(); // Close the file when done reading
    }

    return {items,binDimentions};
}

