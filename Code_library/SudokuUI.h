

#ifndef SUDOKO_DANCING_LINKS_SUDOKUUI_H
#define SUDOKO_DANCING_LINKS_SUDOKUUI_H

#include "SudokuSolver.h"
#include <vector>
#include <string>

class SudokuUI {
public:
    // Display the main menu
    static void displayMenu();

    // Input a puzzle from the user (accepts characters)
    static std::vector<std::vector<int>> inputPuzzle(int size);

    // Helper function to convert character to integer value
    static int charToValue(char c, int maxSize);

    // Helper function to convert integer value to character
    static char valueToChar(int val, int maxSize);

    // Solve a specific Sudoku variant
    static void solveSudokuVariant(SudokuSolver* solver, const std::string& variantName);

    // Run performance comparison across all variants
    static void runPerformanceComparison();

    static std::vector<std::vector<int>> parsePuzzleFromString(const std::vector<std::string>& stringRows, int size);

};



#endif //SUDOKO_DANCING_LINKS_SUDOKUUI_H