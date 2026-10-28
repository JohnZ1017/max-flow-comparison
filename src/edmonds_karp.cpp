#include "edmonds_karp.h"

#include <queue>
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

    std::vector<int> parent(n, -1);

    auto bfs = [&]() -> bool {
        std::fill(parent.begin(), parent.end(), -1);

        std::queue<int> q;
        q.push(source);
        parent[source] = source;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v = 0; v < n; ++v) {
                if (parent[v] == -1 && residual[u][v] > 0) {
                    parent[v] = u;

                    if (v == sink) {
                        return true;
                    }

                    q.push(v);
                }
            }
        }

        return false;
    };

    // Augmentation logic will be added next.
    return 0;
}