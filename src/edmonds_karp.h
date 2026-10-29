#pragma once

#include <vector>
#include "graph.h"

struct EdmondsKarpStats {
    long long bfsSearches = 0;
    long long augmentations = 0;
};

long long edmondsKarp(
    int n,
    const std::vector<EdgeInput>& edges,
    int source,
    int sink,
    EdmondsKarpStats* stats = nullptr
);