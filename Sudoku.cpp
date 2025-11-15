#include "Sudoku.h"
#include <iostream>
#include <cmath>
void printGrid(const Grid &grid) {
    int N = grid.size();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

bool isValid(const Grid &grid, int row, int col, int num) {
    int N = grid.size();
    int sqrtN = sqrt(N);
    for (int x = 0; x < N; x++) {
        if (grid[row][x] == num || grid[x][col] == num)
            return false;
    }
    int startRow = row - row % sqrtN;
    int startCol = col - col % sqrtN;
    for (int i = 0; i < sqrtN; i++)
        for (int j = 0; j < sqrtN; j++)
            if (grid[startRow + i][startCol + j] == num)
                return false;
    return true;
}

bool findEmpty(const Grid &grid, int &row, int &col) {
    int N = grid.size();
    for (row = 0; row < N; row++)
        for (col = 0; col < N; col++)
            if (grid[row][col] == 0)
                return true;
    return false;
}
