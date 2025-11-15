
#include "DancingLinks.h"
#include <climits>

DancingLinks::DancingLinks(int cols) : numColumns(cols), totalNodes(0) {
    header = new Node();
    Node* current = header;

    // Create column headers
    for (int i = 0; i < cols; i++) {
        Node* col = new Node();
        col->size = 0;
        col->column = col;

        current->right = col;
        col->left = current;
        current = col;
        totalNodes++;
    }
    current->right = header;
    header->left = current;
    totalNodes++; // Count header
}

DancingLinks::~DancingLinks() {
    // Clean up all nodes
    Node* col = header->right;
    while (col != header) {
        Node* nextCol = col->right;
        Node* node = col->down;
        while (node != col) {
            Node* nextNode = node->down;
            delete node;
            node = nextNode;
        }
        delete col;
        col = nextCol;
    }
    delete header;
}

void DancingLinks::addRow(const std::vector<int>& rowData, int rowIdx) {
    Node* prev = nullptr;
    Node* first = nullptr;

    Node* col = header->right;
    for (int i = 0; i < numColumns && col != header; i++, col = col->right) {
        if (rowData[i] == 1) {
            Node* newNode = new Node();
            newNode->rowIndex = rowIdx;
            newNode->column = col;

            // Link vertically
            newNode->down = col;
            newNode->up = col->up;
            col->up->down = newNode;
            col->up = newNode;
            col->size++;

            // Link horizontally
            if (prev == nullptr) {
                first = newNode;
            } else {
                prev->right = newNode;
                newNode->left = prev;
            }
            prev = newNode;
            totalNodes++;
        }
    }

    // Close the circular horizontal links
    if (first != nullptr && prev != nullptr) {
        prev->right = first;
        first->left = prev;
    }
}

void DancingLinks::cover(Node* col) {
    metrics.coverUncoverOps++;

    col->right->left = col->left;
    col->left->right = col->right;

    for (Node* row = col->down; row != col; row = row->down) {
        for (Node* node = row->right; node != row; node = node->right) {
            node->down->up = node->up;
            node->up->down = node->down;
            node->column->size--;
        }
    }
}

void DancingLinks::uncover(Node* col) {
    metrics.coverUncoverOps++;

    for (Node* row = col->up; row != col; row = row->up) {
        for (Node* node = row->left; node != row; node = node->left) {
            node->column->size++;
            node->down->up = node;
            node->up->down = node;
        }
    }

    col->right->left = col;
    col->left->right = col;
}

Node* DancingLinks::selectColumn() {
    Node* minCol = nullptr;
    int minSize = INT_MAX;

    for (Node* col = header->right; col != header; col = col->right) {
        if (col->size < minSize) {
            minSize = col->size;
            minCol = col;
        }
    }

    return minCol;
}

void DancingLinks::search(int k) {
    metrics.searchNodes++;
    metrics.currentDepth = k;

    if (k > metrics.maxDepth) {
        metrics.maxDepth = k;
    }

    if (header->right == header) {
        // Found a solution
        std::vector<int> sol;
        for (Node* node : solution) {
            sol.push_back(node->rowIndex);
        }
        solutions.push_back(sol);
        return;
    }

    Node* col = selectColumn();
    if (col == nullptr || col->size == 0) {
        return; // No solution
    }

    cover(col);

    for (Node* row = col->down; row != col; row = row->down) {
        solution.push_back(row);

        for (Node* node = row->right; node != row; node = node->right) {
            cover(node->column);
        }

        search(k + 1);

        row = solution.back();
        solution.pop_back();
        col = row->column;

        for (Node* node = row->left; node != row; node = node->left) {
            uncover(node->column);
        }
    }

    uncover(col);
    metrics.currentDepth = k - 1;
}

std::vector<std::vector<int>> DancingLinks::solve() {
    solutions.clear();
    solution.clear();
    metrics = DLXMetrics(); // Reset metrics
    search(0);
    return solutions;
}

std::vector<int> DancingLinks::getFirstSolution() {
    solutions.clear();
    solution.clear();
    metrics = DLXMetrics(); // Reset metrics
    search(0);
    return solutions.empty() ? std::vector<int>() : solutions[0];
}

size_t DancingLinks::getMemoryUsage() const {
    // Estimate: number of nodes * size of Node structure
    return totalNodes * sizeof(Node);
}