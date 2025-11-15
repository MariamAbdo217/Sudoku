#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>

using namespace std;
typedef vector<vector<int>> Grid;

// Utility functions
void printGrid(const Grid &grid);
bool isValid(const Grid &grid, int row, int col, int num);
bool findEmpty(const Grid &grid, int &row, int &col);

#endif // SUDOKU_H
