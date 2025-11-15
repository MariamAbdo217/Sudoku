cmake_minimum_required(VERSION 4.0)
project(Sudoku)

set(CMAKE_CXX_STANDARD 20)

add_executable(Sudoku main.cpp
        Sudoku.cpp
        Sudoku.h
        NaiveSolver.cpp
        NaiveSolver.h
        ForwardCheckingSolver.cpp
        ForwardCheckingSolver.h
        ConstraintPropagationSolver.cpp
        ConstraintPropagationSolver.h)
