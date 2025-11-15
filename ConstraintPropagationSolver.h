#ifndef CONSTRAINT_PROPAGATION_SOLVER_H
#define CONSTRAINT_PROPAGATION_SOLVER_H

#include "Sudoku.h"

extern int recursiveCalls;
extern int backtrackCount;

bool solveConstraintPropagation(Grid &grid, std::vector<std::vector<std::vector<int>>> &domains);

#endif
