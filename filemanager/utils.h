#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "../item.h"

namespace Utils {

constexpr char OUTPUT_FILE[] = "files/output.txt";
constexpr char FILE_PATH[] = "files/test_files/test_file";

void generate();
int getNumberOfTestFiles();

}
