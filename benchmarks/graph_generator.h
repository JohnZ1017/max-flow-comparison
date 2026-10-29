#pragma once

#include <vector>
#include "../src/graph.h"

std::vector<EdgeInput> generateRandomGraph(
    int vertices,
    int edges,
    long long maxCapacity,
    unsigned int seed
);