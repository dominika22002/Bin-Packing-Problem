#include "algorithm/greedyalgorithm.h"
#include "algorithm/precisealgorithm.h"

#include "filemanager/filemanager.h"
#include "filemanager/utils.h"

#include <chrono>
#include <functional>

namespace
{

    int getTimeOf(std::function<void()> function)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        function();
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

        return static_cast<int>(duration.count());
    }

    void createTimeRaport(int fileNumber)
    {
        int greedyTime = getTimeOf([fileNumber]() { 
                        const auto [items, binDimentions] = FileManager::getValuesFromFile(fileNumber);
                        auto greedyAlgorithm = GreedyAlgorithm(items, binDimentions);
                        greedyAlgorithm.start(); });

        int preciseTime = getTimeOf([fileNumber]() {             
                        const auto [items, binDimentions] = FileManager::getValuesFromFile(fileNumber);
                        auto preciseAlgorithm = PreciseAlgorithm(items, binDimentions);
                        preciseAlgorithm.start(); });

        int multiPreciseTime = getTimeOf([fileNumber]() {             
                        const auto [items, binDimentions] = FileManager::getValuesFromFile(fileNumber);
                        auto preciseAlgorithm = PreciseAlgorithm(items, binDimentions);
                        preciseAlgorithm.startWithMultithreads(); });
        
        std::cout << "\nDla pliku nr " << fileNumber << " :\t" << greedyTime << "\t"<< preciseTime << "\t"<< multiPreciseTime << std::endl;
    
    }
}

int main(int argc, char *argv[])
{
    createTimeRaport(std::atoi(argv[1]));
    return 0;
    std::string choice = "";
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Choose which file you want to use. You have a range [1-"
              << Utils::getNumberOfTestFiles()
              << "] of files. Or you may create new file using 'n' letter" << std::endl;

    choice = argv[1];

    int fileNumber = choice == "n" ? FileManager::createNewTestFile() : std::atoi(choice.c_str());
    const auto [items, binDimentions] = FileManager::getValuesFromFile(fileNumber);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Choose your algorithm: \n"
                 "1 - Greedy Algorithm\n"
                 "2 - Precise Algorithm\n"
                 "3 - Multithreading Precise Algorithm\n" << std::endl;

    choice = argv[2];
    int algorithmNumber = std::atoi(choice.c_str());

    std::vector<Item> packedItems;

    if (algorithmNumber == 1) {
        auto greedyAlgorithm = GreedyAlgorithm(items, binDimentions);
        greedyAlgorithm.start();
        packedItems = greedyAlgorithm.getItems();

    } else if (algorithmNumber == 2) {
        auto preciseAlgorithm = PreciseAlgorithm(items, binDimentions);
        preciseAlgorithm.start();
        packedItems = preciseAlgorithm.getItems();

    } else if (algorithmNumber == 3) {
        auto preciseAlgorithm = PreciseAlgorithm(items, binDimentions);
        preciseAlgorithm.startWithMultithreads();
        packedItems = preciseAlgorithm.getItems();
    }

    FileManager::writeToFile(packedItems, binDimentions);

    return 0;
}



// import consolemenu as menu
// from gui.plot_solved import plot_solved
// from algorithms.greedyalgorithm import greedyAlgorithm
// from algorithms.precisealgorithm import preciseAlgorithm
// from algorithms.differentalgorithm import differentAlgorithm
// import read_write_file.readinputfile as readFileManager

// selectedFilePath = menu.selectFilePath()
// [[bin_width, bin_height], items]= readFileManager.readFromInputFile(
//     selectedFilePath)

// selected_algorithm = menu.selectAlgorithm()
// solved_items = []
// number_of_bins = 0

// if (selected_algorithm == 0):
//     [solved_items, number_of_bins] = greedyAlgorithm(
//         items, bin_width, bin_height)
// elif (selected_algorithm == 1):
//     [solved_items, number_of_bins] = preciseAlgorithm(
//         items, bin_width, bin_height)
// elif (selected_algorithm == 2):
//     [solved_items, number_of_bins] = differentAlgorithm(
//         items, bin_width, bin_height)

// print(solved_items)
// print(number_of_bins)

// print("Try to plot solution!")
// plot_solved(solved_items, [bin_width, bin_height],
//                number_of_bins, selectedFilePath)
