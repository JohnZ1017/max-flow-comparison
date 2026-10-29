#pragma once

#include <vector>
#include "graph.h"

struct DinicStats {
    long long bfsPhases = 0;
    long long blockingFlowPhases = 0;
    long long successfulPushes = 0;
};

long long dinic(
    int n,
    const std::vector<EdgeInput>& edges,
    int source,
    int sink,
    DinicStats* stats = nullptr
);