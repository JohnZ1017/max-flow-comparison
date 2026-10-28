#include "edmonds_karp.h"

#include <vector>

long long edmondsKarp(
    int n,
    const std::vector<EdgeInput>& edges,
    int source,
    int sink
) {
    std::vector<std::vector<long long>> residual(
        n,
        std::vector<long long>(n, 0)
    );

    for (const EdgeInput& edge : edges) {
        residual[edge.from][edge.to] += edge.capacity;
    }

    // BFS and augmentation logic will be added next.
    return 0;
}