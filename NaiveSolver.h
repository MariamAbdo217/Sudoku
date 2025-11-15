#ifndef NAIVE_SOLVER_H
#define NAIVE_SOLVER_H

#include "Sudoku.h"

// Global counters
extern int recursiveCalls;
extern int backtrackCount;

// Solver function
bool solveNaive(Grid &grid);

#endif
