cmake_minimum_required(VERSION 3.0)

project(MyProject)

# Ustaw standard C++ na C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Utwórz zmienną ze wszystkimi plikami źródłowymi
set(SOURCES
    main.cpp

    item.cpp
    item.h

    algorithm/greedyalgorithm.cpp
    algorithm/greedyalgorithm.h
    algorithm/precisealgorithm.cpp
    algorithm/precisealgorithm.h

    filemanager/filemanager.cpp
    filemanager/filemanager.h
    filemanager/utils.cpp
    filemanager/utils.h

    matrix.cpp
    matrix.h
)

# Dodaj wykonywalny plik wyjściowy
add_executable(my_project ${SOURCES}
    filemanager/utils.cpp
    algorithm/precisealgorithm.h algorithm/precisealgorithm.cpp)
