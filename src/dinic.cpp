#include "dinic.h"

#include <algorithm>
#include <queue>
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

    std::vector<int> level(n);

    auto buildLevelGraph = [&]() -> bool {
        std::fill(level.begin(), level.end(), -1);

        std::queue<int> q;
        q.push(source);
        level[source] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const DinicEdge& edge : graph[u]) {
                if (edge.capacity > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }

        return level[sink] != -1;
    };

    // Blocking flow logic will be added next.
    return 0;
}