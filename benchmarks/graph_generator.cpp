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

    // Guarantee at least one source-to-sink path:
    // 0 -> 1 -> 2 -> ... -> vertices - 1
    for (int i = 0; i < vertices - 1; ++i) {
        graph.push_back({
            i,
            i + 1,
            capacityDist(rng)
        });

        usedEdges.insert({i, i + 1});
    }

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

std::vector<EdgeInput> generateLayeredGraph(
    int layers,
    int verticesPerLayer,
    long long maxCapacity,
    unsigned int seed
) {
    std::mt19937 rng(seed);

    std::uniform_int_distribution<long long> capacityDist(
        1,
        maxCapacity
    );

    std::vector<EdgeInput> graph;

    const int source = 0;
    const int firstLayerStart = 1;

    // Connect source to the first layer.
    for (int i = 0; i < verticesPerLayer; ++i) {
        graph.push_back({
            source,
            firstLayerStart + i,
            capacityDist(rng)
        });
    }

    // Connect each layer to the next layer.
    for (int layer = 0; layer < layers - 1; ++layer) {
        int currentLayerStart =
            firstLayerStart + layer * verticesPerLayer;

        int nextLayerStart =
            currentLayerStart + verticesPerLayer;

        for (int u = 0; u < verticesPerLayer; ++u) {
            for (int v = 0; v < verticesPerLayer; ++v) {
                graph.push_back({
                    currentLayerStart + u,
                    nextLayerStart + v,
                    capacityDist(rng)
                });
            }
        }
    }

    const int sink =
        1 + layers * verticesPerLayer;

    const int lastLayerStart =
        firstLayerStart +
        (layers - 1) * verticesPerLayer;

    // Connect the final layer to the sink.
    for (int i = 0; i < verticesPerLayer; ++i) {
        graph.push_back({
            lastLayerStart + i,
            sink,
            capacityDist(rng)
        });
    }

    return graph;
}

std::vector<EdgeInput> generateUnitCapacityGraph(
    int vertices,
    int edges,
    unsigned int seed
) {
    std::mt19937 rng(seed);

    std::uniform_int_distribution<int> vertexDist(
        0,
        vertices - 1
    );

    std::vector<EdgeInput> graph;
    std::set<std::pair<int, int>> usedEdges;

    // Guarantee at least one source-to-sink path:
    // 0 -> 1 -> 2 -> ... -> vertices - 1
    for (int i = 0; i < vertices - 1; ++i) {
        graph.push_back({
            i,
            i + 1,
            1
        });

        usedEdges.insert({i, i + 1});
    }

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
            1
        });
    }

    return graph;
}