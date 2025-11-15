

#ifndef SUDOKO_DANCING_LINKS_SUDOKUSOLVER_H
#define SUDOKO_DANCING_LINKS_SUDOKUSOLVER_H

#include "DancingLinks.h"
#include <vector>
#include <chrono>
#include <iostream>

struct SudokuMetrics {
    double executionTime;           // Total solve time in milliseconds
    double initializationTime;      // Time to build exact-cover matrix in milliseconds
    int searchNodes;                // Total recursive nodes visited
    int coverUncoverOps;           // Number of cover/uncover operations
    int maxRecursionDepth;         // Maximum depth reached in search tree
    size_t memoryUsage;            // Peak memory in bytes
    bool solved;                    // Whether puzzle was solved correctly

    // Constructor with default values
    SudokuMetrics() : executionTime(0), initializationTime(0), searchNodes(0),
                     coverUncoverOps(0), maxRecursionDepth(0), memoryUsage(0),
                     solved(false) {}
};

class SudokuSolver {
protected:
    int gridSize;
    int boxSize;
    std::vector<std::vector<int>> grid;
    SudokuMetrics metrics;

    // Convert Sudoku to exact cover matrix
    void buildExactCoverMatrix(DancingLinks& dlx);

    // Map solution back to Sudoku grid
    void mapSolutionToGrid(const std::vector<int>& solution);

    // Get row number in exact cover matrix
    int getMatrixRow(int row, int col, int num);

    // Calculate memory usage
    size_t calculateMemoryUsage();

public:
    SudokuSolver(int size);

    void loadPuzzle(const std::vector<std::vector<int>>& puzzle);
    bool solve();
    void printGrid() const;
    SudokuMetrics getMetrics() const { return metrics; }
    std::vector<std::vector<int>> getGrid() const { return grid; }
};

// Specific variants
class MiniSudokuSolver : public SudokuSolver {
public:
    MiniSudokuSolver() : SudokuSolver(4) {}
};

class StandardSudokuSolver : public SudokuSolver {
public:
    StandardSudokuSolver() : SudokuSolver(9) {}
};

class ExtendedSudokuSolver : public SudokuSolver {
public:
    ExtendedSudokuSolver() : SudokuSolver(16) {}
};

#endif //SUDOKO_DANCING_LINKS_SUDOKUSOLVER_H