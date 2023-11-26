#include "algorithm/greedyalgorithm.h"
#include "algorithm/precisealgorithm.h"

#include "filemanager/filemanager.h"
#include "filemanager/utils.h"

int main(){
    std::string choice = "";
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Choose which file you want to use. You have a range [1-"
              << Utils::getNumberOfTestFiles()
              << "] of files. Or you may create new file using 'n' letter" << std::endl;

    choice = "5";
    //usunąć - 1, 2, 4, 5, 12, 13, 14, 16, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29,

    int fileNumber = choice == "n" ? FileManager::createNewTestFile() : std::atoi(choice.c_str());
    const auto [items, binDimentions] = FileManager::getValuesFromFile(fileNumber);

    std::cout << "--------------------------------" << std::endl;
    std::cout << "Choose your algorithm: \n"
                 "1 - Greedy Algorithm\n"
                 "2 - Precise Algorithm\n"
                 "3 - Multithreading Precise Algorithm\n" << std::endl;

    choice = "3";
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
