#include "dinic.h"

#include <vector>

struct DinicEdge {
    int to;
    long long capacity;
    int reverseIndex;
};

long long dinic(
    int n,
    const std::vector<EdgeInput>& edges,
    int source,
    int sink
) {
    std::vector<std::vector<DinicEdge>> graph(n);

    auto addEdge = [&](int from, int to, long long capacity) {
        DinicEdge forward{
            to,
            capacity,
            static_cast<int>(graph[to].size())
        };

        DinicEdge reverse{
            from,
            0,
            static_cast<int>(graph[from].size())
        };

        graph[from].push_back(forward);
        graph[to].push_back(reverse);
    };

    for (const EdgeInput& edge : edges) {
        addEdge(edge.from, edge.to, edge.capacity);
    }

    // Level graph and blocking-flow logic will be added next.
    return 0;
}