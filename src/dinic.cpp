#include "dinic.h"

#include <algorithm>
#include <limits>
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
    int sink,
    DinicStats* stats
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
        if (stats != nullptr) {
            stats->bfsPhases++;
        }

        std::fill(level.begin(), level.end(), -1);

        std::queue<int> q;
        q.push(source);
        level[source] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (const DinicEdge& edge : graph[u]) {
                if (edge.capacity > 0 &&
                    level[edge.to] == -1) {

                    level[edge.to] = level[u] + 1;
                    q.push(edge.to);
                }
            }
        }

        return level[sink] != -1;
    };

    std::vector<int> nextEdge(n);

    auto sendFlow = [&](auto&& self,
                        int u,
                        long long flow) -> long long {

        if (u == sink) {
            return flow;
        }

        for (int& i = nextEdge[u];
             i < static_cast<int>(graph[u].size());
             ++i) {

            DinicEdge& edge = graph[u][i];

            if (edge.capacity <= 0) {
                continue;
            }

            if (level[edge.to] != level[u] + 1) {
                continue;
            }

            long long pushed = self(
                self,
                edge.to,
                std::min(flow, edge.capacity)
            );

            if (pushed > 0) {
                edge.capacity -= pushed;
                graph[edge.to][edge.reverseIndex].capacity += pushed;

                if (stats != nullptr) {
                    stats->successfulPushes++;
                }

                return pushed;
            }
        }

        return 0;
    };

    long long maxFlow = 0;

    while (buildLevelGraph()) {
        if (stats != nullptr) {
            stats->blockingFlowPhases++;
        }

        std::fill(
            nextEdge.begin(),
            nextEdge.end(),
            0
        );

        while (true) {
            long long pushed = sendFlow(
                sendFlow,
                source,
                std::numeric_limits<long long>::max()
            );

            if (pushed == 0) {
                break;
            }

            maxFlow += pushed;
        }
    }

    return maxFlow;
}