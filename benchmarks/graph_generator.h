#pragma once

#include <vector>
#include "../src/graph.h"

std::vector<EdgeInput> generateRandomGraph(
    int vertices,
    int edges,
    long long maxCapacity,
    unsigned int seed
);

std::vector<EdgeInput> generateLayeredGraph(
    int layers,
    int verticesPerLayer,
    long long maxCapacity,
    unsigned int seed
);

std::vector<EdgeInput> generateUnitCapacityGraph(
    int vertices,
    int edges,
    unsigned int seed
);