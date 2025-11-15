#include "NaiveSolver.h"
#include "ForwardCheckingSolver.h"
#include "ConstraintPropagationSolver.h"
#include "Sudoku.h"
#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
using namespace chrono;

// Function to reset global counters
void resetCounters() {
    recursiveCalls = 0;
    backtrackCount = 0;
}

int main() {
    // Example puzzles for 4x4, 9x9, and 16x16
    vector<vector<vector<int>>> puzzles4x4 = {
        {{1,0,0,4},
         {0,0,0,2},
         {0,1,0,0},
         {2,0,4,0}}
    };

    vector<vector<vector<int>>> puzzles9x9 = {
        {{5,3,0,0,7,0,0,0,0},
         {6,0,0,1,9,5,0,0,0},
         {0,9,8,0,0,0,0,6,0},
         {8,0,0,0,6,0,0,0,3},
         {4,0,0,8,0,3,0,0,1},
         {7,0,0,0,2,0,0,0,6},
         {0,6,0,0,0,0,2,8,0},
         {0,0,0,4,1,9,0,0,5},
         {0,0,0,0,8,0,0,7,9}}
    };

    vector<vector<vector<int>>> puzzles16x16 = {
        {{0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1},
         {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,2,0},
         {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,3,0,0},
         {0,0,0,0, 0,0,0,0, 0,0,0,0, 4,0,0,0},

         {0,0,0,0, 0,0,0,0, 0,0,0,5, 0,0,0,0},
         {0,0,0,0, 0,0,0,0, 0,0,6,0, 0,0,0,0},
         {0,0,0,0, 0,0,0,0, 0,7,0,0, 0,0,0,0},
         {0,0,0,0, 0,0,0,0, 8,0,0,0, 0,0,0,0},

         {0,0,0,0, 0,0,9,0, 0,0,0,0, 0,0,0,0},
         {0,0,0,0, 0,10,0,0, 0,0,0,0, 0,0,0,0},
         {0,0,0,0, 11,0,0,0, 0,0,0,0, 0,0,0,0},
         {0,0,0,12,0,0,0,0, 0,0,0,0, 0,0,0,0},

         {0,0,13,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
         {0,14,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
         {15,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0},
         {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0}}
    };

    // Loop over puzzle sets
    vector<pair<string, vector<vector<vector<int>>>>> allPuzzles = {
        {"4x4", puzzles4x4},
        {"9x9", puzzles9x9},
        {"16x16", puzzles16x16}
    };

    for (auto &[sizeName, puzzleSet] : allPuzzles) {
        cout << "==================== " << sizeName << " Puzzles ====================\n";

        for (size_t i = 0; i < puzzleSet.size(); i++) {
            cout << "--- Puzzle " << i+1 << " ---\n";
            Grid puzzle = puzzleSet[i];

            // Print original puzzle
            cout << "Original puzzle:\n";
            printGrid(puzzle);
            cout << endl;

            // ================= Naive Solver =================
            resetCounters();
            auto start = high_resolution_clock::now();
            Grid puzzleCopy = puzzle;
            if (solveNaive(puzzleCopy)) {
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end-start).count();
                cout << "[Naive Backtracking] Solved in " << duration << " ms\n";
                cout << "Recursive calls: " << recursiveCalls
                     << ", Backtracks: " << backtrackCount << endl;
            } else {
                cout << "[Naive Backtracking] No solution found.\n";
            }
            cout << endl;

            // ================= Forward Checking Solver =================
            resetCounters();
            puzzleCopy = puzzle;
            vector<vector<vector<int>>> domains(puzzleCopy.size(),
                                                vector<vector<int>>(puzzleCopy.size()));
            for (int r = 0; r < (int)puzzleCopy.size(); r++)
                for (int c = 0; c < (int)puzzleCopy.size(); c++)
                    if (puzzleCopy[r][c] == 0)
                        for (int num = 1; num <= (int)puzzleCopy.size(); num++)
                            if (isValid(puzzleCopy, r, c, num))
                                domains[r][c].push_back(num);
                    else
                        domains[r][c].push_back(puzzleCopy[r][c]);

            start = high_resolution_clock::now();
            if (solveForwardChecking(puzzleCopy, domains)) {
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end-start).count();
                cout << "[Forward Checking] Solved in " << duration << " ms\n";
                cout << "Recursive calls: " << recursiveCalls
                     << ", Backtracks: " << backtrackCount << endl;
            } else {
                cout << "[Forward Checking] No solution found.\n";
            }
            cout << endl;

            // ================= Constraint Propagation Solver =================
            resetCounters();
            puzzleCopy = puzzle;
            vector<vector<vector<int>>> cpDomains(puzzleCopy.size(),
                                                  vector<vector<int>>(puzzleCopy.size()));
            for (int r = 0; r < (int)puzzleCopy.size(); r++)
                for (int c = 0; c < (int)puzzleCopy.size(); c++)
                    if (puzzleCopy[r][c] == 0)
                        for (int num = 1; num <= (int)puzzleCopy.size(); num++)
                            if (isValid(puzzleCopy, r, c, num))
                                cpDomains[r][c].push_back(num);
                    else
                        cpDomains[r][c].push_back(puzzleCopy[r][c]);

            start = high_resolution_clock::now();
            if (solveConstraintPropagation(puzzleCopy, cpDomains)) {
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end-start).count();
                cout << "[Constraint Propagation] Solved in " << duration << " ms\n";
                cout << "Recursive calls: " << recursiveCalls
                     << ", Backtracks: " << backtrackCount << endl;
            } else {
                cout << "[Constraint Propagation] No solution found.\n";
            }

            cout << "-------------------------------------------\n\n";
        }
    }

    return 0;
}
