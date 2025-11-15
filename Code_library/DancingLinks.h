

#ifndef SUDOKO_DANCING_LINKS_DANCINGLINKS_H
#define SUDOKO_DANCING_LINKS_DANCINGLINKS_H

#include <vector>
#include <memory>
#include <algorithm>

// Node structure for the Dancing Links sparse matrix representation
struct Node {
    Node* left;
    Node* right;
    Node* up;
    Node* down;
    Node* column;
    int rowIndex;
    int size; // Only used for column headers

    Node() : left(this), right(this), up(this), down(this),
             column(nullptr), rowIndex(-1), size(0) {}
};

// Metrics tracking structure
struct DLXMetrics {
    int searchNodes;
    int coverUncoverOps;
    int maxDepth;
    int currentDepth;

    DLXMetrics() : searchNodes(0), coverUncoverOps(0), maxDepth(0), currentDepth(0) {}
};

class DancingLinks {
private:
    Node* header;
    std::vector<Node*> solution;
    std::vector<std::vector<int>> solutions;
    int numColumns;
    int totalNodes;
    DLXMetrics metrics;

    // Core Dancing Links operations
    void cover(Node* col);
    void uncover(Node* col);
    void search(int k);

    // Helper to select the column with minimum size (heuristic)
    Node* selectColumn();

public:
    DancingLinks(int cols);
    ~DancingLinks();

    // Add a row to the sparse matrix
    void addRow(const std::vector<int>& rowData, int rowIdx);

    // Solve and return all solutions
    std::vector<std::vector<int>> solve();

    // Get the first solution only
    std::vector<int> getFirstSolution();

    // Get metrics
    DLXMetrics getMetrics() const { return metrics; }

    // Get memory usage estimate
    size_t getMemoryUsage() const;
};

#endif //SUDOKO_DANCING_LINKS_DANCINGLINKS_H