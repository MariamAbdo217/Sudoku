#include "ForwardCheckingSolver.h"
#include "Sudoku.h"
#include <cmath>
#include <algorithm>  // for std::remove

// Forward Checking Solver
bool solveForwardChecking(Grid &grid, vector<vector<vector<int>>> &domains) {
    recursiveCalls++;
    int row, col;
    int N = grid.size();

    if (!findEmpty(grid, row, col))
        return true;

    // Try all possible values in the domain for this cell
    for (int num : domains[row][col]) {
        if (isValid(grid, row, col, num)) {
            grid[row][col] = num;

            // Backup domains to restore after recursion
            auto backupDomains = domains;

            // Forward checking: remove num from domains of related cells
            int sqrtN = sqrt(N);
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

            // Check if any cell has empty domain → backtrack early
            bool failed = false;
            for (int r = 0; r < N && !failed; r++)
                for (int c = 0; c < N && !failed; c++)
                    if (grid[r][c] == 0 && domains[r][c].empty())
                        failed = true;

            if (!failed && solveForwardChecking(grid, domains))
                return true;

            // Backtrack
            grid[row][col] = 0;
            domains = backupDomains;
            backtrackCount++;
        }
    }

    return false;
}
