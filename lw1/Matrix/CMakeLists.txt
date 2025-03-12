cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 20)

project(MultiMatrix)

add_executable(
        MultiMatrix
        MultiMatrix.cpp
        Matrix.cpp
        MatrixRow.cpp
        ../../tools/CLIParser.cpp
)

project(InvertMatrix)

add_executable(
        InvertMatrix
        InvertMatrix.cpp
        Matrix.cpp
        MatrixRow.cpp
        ../../tools/CLIParser.cpp
)

include_directories(
    ../../tools
)


