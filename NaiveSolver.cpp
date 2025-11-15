#include "NaiveSolver.h"

int recursiveCalls = 0;
int backtrackCount = 0;

bool solveNaive(Grid &grid) {
    recursiveCalls++;
    int row, col;
    if (!findEmpty(grid, row, col)) return true;

    int N = grid.size();
    for (int num = 1; num <= N; num++) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;
            if (solveNaive(grid)) return true;
            grid[row][col] = 0;
            backtrackCount++;
        }
    }
    return false;
}
