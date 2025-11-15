

#include "SudokuUI.h"
#include <iostream>
#include <iomanip>
#include <cctype>

using namespace std;

void SudokuUI::displayMenu() {
    cout << "\n===== Sudoku Solver using Dancing Links =====\n";
    cout << "1. Solve 4x4 Mini Sudoku\n";
    cout << "2. Solve 9x9 Standard Sudoku\n";
    cout << "3. Solve 16x16 Extended Sudoku\n";
    cout << "4. Run Performance Comparison\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

int SudokuUI::charToValue(char c, int maxSize) {
    if (c == '0' || c == '.' || c == '-') {
        return 0; // Empty cell
    }

    if (maxSize <= 9) {
        // For 4x4 and 9x9: accept digits 1-9
        if (c >= '1' && c <= '9') {
            return c - '0';
        }
    } else {
        // For 16x16: accept 1-9 and A-G (or a-g)
        if (c >= '1' && c <= '9') {
            return c - '0';
        } else if (c >= 'A' && c <= 'G') {
            return 10 + (c - 'A');
        } else if (c >= 'a' && c <= 'g') {
            return 10 + (c - 'a');
        }
    }

    return -1; // Invalid input
}


char SudokuUI::valueToChar(int val, int maxSize) {
    if (val == 0) {
        return '.';
    }

    if (maxSize <= 9) {
        return '0' + val;
    } else {
        // For 16x16
        if (val <= 9) {
            return '0' + val;
        } else if (val <= 16) {
            return 'A' + (val - 10);
        }
    }
    return '?'; // Should not reach here
}


vector<vector<int>> SudokuUI::inputPuzzle(int size) {
    vector<vector<int>> puzzle(size, vector<int>(size));

    cout << "\nEnter the puzzle:\n";
    if (size == 4) {
        cout << "Use 1-4 for filled cells, 0 or . for empty cells\n";
    } else if (size == 9) {
        cout << "Use 1-9 for filled cells, 0 or . for empty cells\n";
    } else {
        cout << "Use 1-9 and A-F for filled cells, 0 or . for empty cells\n";
    }
    cout << "Enter " << size << " characters per row (no spaces):\n\n";

    for (int i = 0; i < size; i++) {
        string row;
        bool validInput = false;

        while (!validInput) {
            cout << "Row " << (i + 1) << ": ";
            cin >> row;

            // Remove any spaces
            row.erase(remove(row.begin(), row.end(), ' '), row.end());

            if (row.length() != static_cast<size_t>(size)) {
                cout << "Invalid input! Please enter exactly " << size << " characters.\n";
                continue;
            }

            validInput = true;
            for (int j = 0; j < size; j++) {
                int val = charToValue(row[j], size);
                if (val < 0 || val > size) {
                    cout << "Invalid character '" << row[j] << "' at position " << (j + 1) << "!\n";
                    validInput = false;
                    break;
                }
                puzzle[i][j] = val;
            }
        }
    }

    return puzzle;
}

void SudokuUI::solveSudokuVariant(SudokuSolver* solver, const string& variantName) {
    cout << "\n===== " << variantName << " =====\n";

    int size = (variantName == "4x4 Mini Sudoku") ? 4 :
               (variantName == "9x9 Standard Sudoku") ? 9 : 16;

    vector<vector<int>> puzzle = inputPuzzle(size);
    solver->loadPuzzle(puzzle);

    cout << "\nOriginal Puzzle:\n";
    solver->printGrid();

    cout << "\nSolving...\n";
    bool solved = solver->solve();

    if (solved) {
        cout << "\nSolved Puzzle:\n";
        solver->printGrid();

        SudokuMetrics metrics = solver->getMetrics();
        cout << "\n========== Performance Metrics ==========\n";
        cout << fixed << setprecision(3);
        cout << "Execution Time:           " << metrics.executionTime << " ms\n";
        cout << "Initialization Time:      " << metrics.initializationTime << " ms\n";
        cout << "Search Nodes:             " << metrics.searchNodes << "\n";
        cout << "Cover/Uncover Operations: " << metrics.coverUncoverOps << "\n";
        cout << "Maximum Recursion Depth:  " << metrics.maxRecursionDepth << "\n";
        cout << "Memory Usage:             " << (metrics.memoryUsage / 1024.0) << " KB\n";
        cout << "Accuracy:                 " << (metrics.solved ? "100%" : "0%") << "\n";
        cout << "=========================================\n";
    } else {
        cout << "\nNo solution found for this puzzle.\n";
    }
}

vector<vector<int>> SudokuUI::parsePuzzleFromString(const vector<string>& stringRows, int size) {
    vector<vector<int>> puzzle(size, vector<int>(size, 0));

    for (int i = 0; i < size && i < (int)stringRows.size(); i++) {
        string row = stringRows[i];
        int col = 0;

        for (size_t j = 0; j < row.length() && col < size; j++) {
            char c = row[j];

            if (c == ' ' || c == ',' || c == '{' || c == '}') {
                continue;
            }

            int value = charToValue(c, size);
            if (value >= 0) {
                puzzle[i][col] = value;
                col++;
            }
        }
    }

    return puzzle;
}


void SudokuUI::runPerformanceComparison() {
    cout << "\n===== Performance Comparison Across Variants =====\n\n";

    vector<string> mini4x4_str = {
        "0312",
        "2004",
        "3001",
        "0040"
    };

    vector<string> standard9x9_str = {
        "500170000",
        "010502038",
        "008040570",
        "762000053",
        "000060080",
        "301900000",
        "409720000",
        "000000000",
        "870004009"
    };

    vector<string> extended16x16_str = {
        "02035000D000C00B",
        "060002C008F340E0",
        "00CE048B00G00000",
        "000B000020A006G",
        "0E800040C0D00001",
        "00503F0090016E0C",
        "F000CB610E00A000",
        "001980000040GFB0",
        "007000000000000F",
        "20BD6E003070005A",
        "00004805000006D0",
        "3AF810006G902B00",
        "E06F0C0800100A20",
        "00G00100F0000000",
        "000003DE2507F008",
        "B420F0A68C0D0105"
    };

    vector<vector<int>> mini4x4 = parsePuzzleFromString(mini4x4_str, 4);
    vector<vector<int>> standard9x9 = parsePuzzleFromString(standard9x9_str, 9);
    vector<vector<int>> extended16x16 = parsePuzzleFromString(extended16x16_str, 16);

    cout << "Testing 4x4 Mini Sudoku...\n";
    MiniSudokuSolver miniSolver;
    miniSolver.loadPuzzle(mini4x4);
    miniSolver.solve();
    auto miniMetrics = miniSolver.getMetrics();
    cout << "4x4 solved in " << fixed << setprecision(3) << miniMetrics.executionTime << " ms\n\n";

    cout << "Testing 9x9 Standard Sudoku...\n";
    StandardSudokuSolver standardSolver;
    standardSolver.loadPuzzle(standard9x9);
    standardSolver.solve();
    auto standardMetrics = standardSolver.getMetrics();
    cout << "9x9 solved in " << fixed << setprecision(3) << standardMetrics.executionTime << " ms\n\n";

    cout << "Testing 16x16 Extended Sudoku...\n";
    ExtendedSudokuSolver extendedSolver;
    extendedSolver.loadPuzzle(extended16x16);
    extendedSolver.solve();
    auto extendedMetrics = extendedSolver.getMetrics();
    cout << "16x16 solved in " << fixed << setprecision(3) << extendedMetrics.executionTime << " ms\n\n";

    cout << "\n" << string(120, '=') << "\n";
    cout << setw(15) << "Variant"
         << setw(15) << "Time (ms)"
         << setw(15) << "Init (ms)"
         << setw(15) << "Nodes"
         << setw(20) << "Cover/Uncover"
         << setw(15) << "Max Depth"
         << setw(15) << "Memory (KB)" << "\n";
    cout << string(120, '-') << "\n";

    cout << setw(15) << "4x4 Mini"
         << setw(15) << fixed << setprecision(3) << miniMetrics.executionTime
         << setw(15) << miniMetrics.initializationTime
         << setw(15) << miniMetrics.searchNodes
         << setw(20) << miniMetrics.coverUncoverOps
         << setw(15) << miniMetrics.maxRecursionDepth
         << setw(15) << setprecision(2) << (miniMetrics.memoryUsage / 1024.0) << "\n";

    cout << setw(15) << "9x9 Standard"
         << setw(15) << setprecision(3) << standardMetrics.executionTime
         << setw(15) << standardMetrics.initializationTime
         << setw(15) << standardMetrics.searchNodes
         << setw(20) << standardMetrics.coverUncoverOps
         << setw(15) << standardMetrics.maxRecursionDepth
         << setw(15) << setprecision(2) << (standardMetrics.memoryUsage / 1024.0) << "\n";

    cout << setw(15) << "16x16 Extended"
         << setw(15) << setprecision(3) << extendedMetrics.executionTime
         << setw(15) << extendedMetrics.initializationTime
         << setw(15) << extendedMetrics.searchNodes
         << setw(20) << extendedMetrics.coverUncoverOps
         << setw(15) << extendedMetrics.maxRecursionDepth
         << setw(15) << setprecision(2) << (extendedMetrics.memoryUsage / 1024.0) << "\n";

    cout << string(120, '=') << "\n";

    cout << "\n===== Scalability Analysis =====\n";

    double timeScale_4to9 = (miniMetrics.executionTime > 0) ?
        (standardMetrics.executionTime / miniMetrics.executionTime) : 0;
    double timeScale_9to16 = (standardMetrics.executionTime > 0) ?
        (extendedMetrics.executionTime / standardMetrics.executionTime) : 0;
    double memScale_4to9 = (miniMetrics.memoryUsage > 0) ?
        (standardMetrics.memoryUsage / (double)miniMetrics.memoryUsage) : 0;
    double memScale_9to16 = (standardMetrics.memoryUsage > 0) ?
        (extendedMetrics.memoryUsage / (double)standardMetrics.memoryUsage) : 0;

    cout << "Time scaling (4x4 -> 9x9):     " << setprecision(2) << timeScale_4to9 << "x\n";
    cout << "Time scaling (9x9 -> 16x16):   " << setprecision(2) << timeScale_9to16 << "x\n";
    cout << "Memory scaling (4x4 -> 9x9):   " << setprecision(2) << memScale_4to9 << "x\n";
    cout << "Memory scaling (9x9 -> 16x16): " << setprecision(2) << memScale_9to16 << "x\n";
    cout << "================================\n";
}



