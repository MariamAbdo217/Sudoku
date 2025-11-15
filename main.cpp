#include "SudokuUI.h"
#include "SudokuSolver.h"
#include <iostream>

int main() {
    int choice;

    while (true) {
        SudokuUI::displayMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
                MiniSudokuSolver solver;
                SudokuUI::solveSudokuVariant(&solver, "4x4 Mini Sudoku");
                break;
        }
        case 2: {
                StandardSudokuSolver solver;
                SudokuUI::solveSudokuVariant(&solver, "9x9 Standard Sudoku");
                break;
        }
        case 3: {
                ExtendedSudokuSolver solver;
                SudokuUI::solveSudokuVariant(&solver, "16x16 Extended Sudoku");
                break;
        }
        case 4:
            SudokuUI::runPerformanceComparison();
            break;
        case 5:
            std::cout << "\nThank you for using the Sudoku Solver!\n";
            return 0;
        default:
            std::cout << "\nInvalid choice. Please try again.\n";
        }
    }

    return 0;
}
