#include "graph_generator.h"

#include <random>
#include <set>
#include <utility>

std::vector<EdgeInput> generateRandomGraph(
    int vertices,
    int edges,
    long long maxCapacity,
    unsigned int seed
) {
    std::mt19937 rng(seed);

    std::uniform_int_distribution<int> vertexDist(0, vertices - 1);
    std::uniform_int_distribution<long long> capacityDist(1, maxCapacity);

    std::vector<EdgeInput> graph;
    std::set<std::pair<int, int>> usedEdges;

    while (static_cast<int>(graph.size()) < edges) {
        int from = vertexDist(rng);
        int to = vertexDist(rng);

        if (from == to) {
            continue;
        }

        if (usedEdges.count({from, to}) > 0) {
            continue;
        }

        usedEdges.insert({from, to});

        graph.push_back({
            from,
            to,
            capacityDist(rng)
        });
    }

    return graph;
}