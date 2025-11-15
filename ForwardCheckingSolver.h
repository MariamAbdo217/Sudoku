#ifndef FORWARD_CHECKING_SOLVER_H
#define FORWARD_CHECKING_SOLVER_H

#include "Sudoku.h"

extern int recursiveCalls;
extern int backtrackCount;

bool solveForwardChecking(Grid &grid, std::vector<std::vector<std::vector<int>>> &domains);

#endif
