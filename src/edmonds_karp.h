#pragma once

#include <vector>
#include "graph.h"

long long edmondsKarp(
    int n,
    const std::vector<EdgeInput>& edges,
    int source,
    int sink
);