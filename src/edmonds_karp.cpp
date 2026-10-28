#include "edmonds_karp.h"

#include <algorithm>
#include <limits>
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

    long long maxFlow = 0;

    while (bfs()) {
        long long bottleneck = std::numeric_limits<long long>::max();

        // Walk backwards from sink to source
        // and find the smallest residual capacity.
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            bottleneck = std::min(bottleneck, residual[u][v]);
        }

        // Update the residual graph.
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];

            residual[u][v] -= bottleneck;
            residual[v][u] += bottleneck;
        }

        maxFlow += bottleneck;
    }

    return maxFlow;
}