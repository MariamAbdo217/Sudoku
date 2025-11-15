
#include "SudokuSolver.h"
#include <cmath>
#include <iomanip>
using namespace std;

SudokuSolver::SudokuSolver(int size)
    : gridSize(size), boxSize(static_cast<int>(sqrt(size))) {
    grid.resize(gridSize, vector<int>(gridSize, 0));
}

void SudokuSolver::loadPuzzle(const vector<vector<int>>& puzzle) {
    grid = puzzle;
}

int SudokuSolver::getMatrixRow(int row, int col, int num) {
    return row * gridSize * gridSize + col * gridSize + num;
}

void SudokuSolver::buildExactCoverMatrix(DancingLinks& dlx) {
    // For each possible placement (row, col, num)
    for (int row = 0; row < gridSize; row++) {
        for (int col = 0; col < gridSize; col++) {
            for (int num = 1; num <= gridSize; num++) {
                // Skip if cell already filled with different number
                if (grid[row][col] != 0 && grid[row][col] != num) {
                    continue;
                }

                vector<int> matrixRow(4 * gridSize * gridSize, 0);
                int rowIdx = getMatrixRow(row, col, num - 1);

                // Constraint 1: Cell constraint
                int cellConstraint = row * gridSize + col;
                matrixRow[cellConstraint] = 1;

                // Constraint 2: Row constraint
                int rowConstraint = gridSize * gridSize + row * gridSize + (num - 1);
                matrixRow[rowConstraint] = 1;

                // Constraint 3: Column constraint
                int colConstraint = 2 * gridSize * gridSize + col * gridSize + (num - 1);
                matrixRow[colConstraint] = 1;

                // Constraint 4: Box constraint
                int boxRow = row / boxSize;
                int boxCol = col / boxSize;
                int boxNum = boxRow * boxSize + boxCol;
                int boxConstraint = 3 * gridSize * gridSize + boxNum * gridSize + (num - 1);
                matrixRow[boxConstraint] = 1;

                dlx.addRow(matrixRow, rowIdx);
            }
        }
    }
}

void SudokuSolver::mapSolutionToGrid(const vector<int>& solution) {
    for (int rowIdx : solution) {
        int num = (rowIdx % gridSize) + 1;
        int col = (rowIdx / gridSize) % gridSize;
        int row = rowIdx / (gridSize * gridSize);
        grid[row][col] = num;
    }
}

size_t SudokuSolver::calculateMemoryUsage() {
    // Grid memory
    size_t gridMemory = gridSize * gridSize * sizeof(int);
    return gridMemory;
}

bool SudokuSolver::solve() {
    auto startTime = chrono::high_resolution_clock::now();

    // Create exact cover matrix: 4 * N^2 constraints
    int numConstraints = 4 * gridSize * gridSize;
    DancingLinks dlx(numConstraints);

    auto initStartTime = chrono::high_resolution_clock::now();

    // Build the exact cover matrix
    buildExactCoverMatrix(dlx);

    auto initEndTime = chrono::high_resolution_clock::now();
    metrics.initializationTime = chrono::duration<double, milli>(
        initEndTime - initStartTime).count();

    // Solve using Dancing Links
    vector<int> solution = dlx.getFirstSolution();

    auto endTime = chrono::high_resolution_clock::now();

    // Get DLX metrics
    DLXMetrics dlxMetrics = dlx.getMetrics();
    metrics.searchNodes = dlxMetrics.searchNodes;
    metrics.coverUncoverOps = dlxMetrics.coverUncoverOps;
    metrics.maxRecursionDepth = dlxMetrics.maxDepth;

    // Calculate memory usage
    metrics.memoryUsage = calculateMemoryUsage() + dlx.getMemoryUsage();

    if (!solution.empty()) {
        mapSolutionToGrid(solution);
        metrics.solved = true;

        // Calculate total execution time
        metrics.executionTime = chrono::duration<double, milli>(
            endTime - startTime).count();

        return true;
    }

    metrics.solved = false;
    metrics.executionTime = chrono::duration<double, milli>(
        endTime - startTime).count();
    return false;
}

void SudokuSolver::printGrid() const {
    int fieldWidth = (gridSize > 9) ? 3 : 2;

    for (int row = 0; row < gridSize; row++) {
        if (row > 0 && row % boxSize == 0) {
            for (int i = 0; i < gridSize * fieldWidth + boxSize + 1; i++) {
                cout << "-";
            }
            cout << endl;
        }

        for (int col = 0; col < gridSize; col++) {
            if (col > 0 && col % boxSize == 0) {
                cout << "| ";
            }

            if (grid[row][col] == 0) {
                cout << setw(fieldWidth) << ".";
            } else {
                if (gridSize == 16) {
                    // Display using hexadecimal for 16x16 (1-9, A-G)
                    if (grid[row][col] <= 9) {
                        cout << setw(fieldWidth) << grid[row][col];
                    } else if (grid[row][col] <= 16) {
                        char hexChar = 'A' + (grid[row][col] - 10);
                        cout << setw(fieldWidth) << hexChar;
                    }
                } else {
                    cout << setw(fieldWidth) << grid[row][col];
                }
            }
        }
        cout << endl;
    }
}

