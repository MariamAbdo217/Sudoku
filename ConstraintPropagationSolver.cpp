#include "ConstraintPropagationSolver.h"
#include "Sudoku.h"
#include <cmath>
#include <algorithm>

// Constraint Propagation Solver
bool solveConstraintPropagation(Grid &grid, vector<vector<vector<int>>> &domains) {
    recursiveCalls++;
    int N = grid.size();
    int row, col;

    // Find next empty cell
    if (!findEmpty(grid, row, col))
        return true;

    // Forward checking and constraint propagation combined
    for (int num : domains[row][col]) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;
            auto backupDomains = domains;

            int sqrtN = sqrt(N);
            // Remove num from domains of related cells
            for (int k = 0; k < N; k++) {
                // Row
                domains[row][k].erase(
                    remove(domains[row][k].begin(), domains[row][k].end(), num),
                    domains[row][k].end()
                );
                // Column
                domains[k][col].erase(
                    remove(domains[k][col].begin(), domains[k][col].end(), num),
                    domains[k][col].end()
                );
            }
            // Subgrid
            int startRow = row - row % sqrtN;
            int startCol = col - col % sqrtN;
            for (int i = 0; i < sqrtN; i++)
                for (int j = 0; j < sqrtN; j++)
                    domains[startRow + i][startCol + j].erase(
                        remove(domains[startRow + i][startCol + j].begin(),
                               domains[startRow + i][startCol + j].end(), num),
                        domains[startRow + i][startCol + j].end()
                    );

            // Constraint propagation: assign any forced cells
            bool changed;
            do {
                changed = false;
                for (int r = 0; r < N; r++) {
                    for (int c = 0; c < N; c++) {
                        if (grid[r][c] == 0 && domains[r][c].size() == 1) {
                            int forcedNum = domains[r][c][0];
                            grid[r][c] = forcedNum;
                            changed = true;

                            // Remove forcedNum from related cells
                            for (int k = 0; k < N; k++) {
                                domains[r][k].erase(
                                    remove(domains[r][k].begin(), domains[r][k].end(), forcedNum),
                                    domains[r][k].end()
                                );
                                domains[k][c].erase(
                                    remove(domains[k][c].begin(), domains[k][c].end(), forcedNum),
                                    domains[k][c].end()
                                );
                            }
                            for (int i = 0; i < sqrtN; i++)
                                for (int j = 0; j < sqrtN; j++)
                                    domains[startRow + i][startCol + j].erase(
                                        remove(domains[startRow + i][startCol + j].begin(),
                                               domains[startRow + i][startCol + j].end(), forcedNum),
                                        domains[startRow + i][startCol + j].end()
                                    );
                        }
                    }
                }
            } while (changed);

            // Check for any empty domain → backtrack early
            bool failed = false;
            for (int r = 0; r < N && !failed; r++)
                for (int c = 0; c < N && !failed; c++)
                    if (grid[r][c] == 0 && domains[r][c].empty())
                        failed = true;

            if (!failed && solveConstraintPropagation(grid, domains))
                return true;

            // Backtrack
            grid[row][col] = 0;
            domains = backupDomains;
            backtrackCount++;
        }
    }

    return false;
}
