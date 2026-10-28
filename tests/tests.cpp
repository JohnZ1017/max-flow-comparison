#include <cassert>
#include <iostream>
#include <vector>

#include "../src/graph.h"
#include "../src/edmonds_karp.h"

void testSimpleGraph() {
    std::vector<EdgeInput> edges = {
        {0, 1, 3},
        {0, 2, 2},
        {1, 2, 1},
        {1, 3, 2},
        {2, 3, 4}
    };

    long long result = edmondsKarp(4, edges, 0, 3);

    assert(result == 5);
}

void testSinglePath() {
    std::vector<EdgeInput> edges = {
        {0, 1, 5},
        {1, 2, 3},
        {2, 3, 4}
    };

    long long result = edmondsKarp(4, edges, 0, 3);

    assert(result == 3);
}

void testNoPath() {
    std::vector<EdgeInput> edges = {
        {0, 1, 5},
        {2, 3, 7}
    };

    long long result = edmondsKarp(4, edges, 0, 3);

    assert(result == 0);
}

void testParallelEdges() {
    std::vector<EdgeInput> edges = {
        {0, 1, 3},
        {0, 1, 4},
        {1, 2, 10}
    };

    long long result = edmondsKarp(3, edges, 0, 2);

    assert(result == 7);
}

int main() {
    testSimpleGraph();
    testSinglePath();
    testNoPath();
    testParallelEdges();

    std::cout << "All Edmonds-Karp tests passed.\n";

    return 0;
}