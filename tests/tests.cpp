#include <cassert>
#include <iostream>
#include <vector>

#include "../src/graph.h"
#include "../src/edmonds_karp.h"
#include "../src/dinic.h"

void testSimpleGraph() {
    std::vector<EdgeInput> edges = {
        {0, 1, 3},
        {0, 2, 2},
        {1, 2, 1},
        {1, 3, 2},
        {2, 3, 4}
    };

    long long ek = edmondsKarp(4, edges, 0, 3);
    long long dn = dinic(4, edges, 0, 3);

    assert(ek == 5);
    assert(dn == 5);
    assert(ek == dn);

    std::cout << "testSimpleGraph passed\n";
}

void testSinglePath() {
    std::vector<EdgeInput> edges = {
        {0, 1, 5},
        {1, 2, 3},
        {2, 3, 4}
    };

    long long ek = edmondsKarp(4, edges, 0, 3);
    long long dn = dinic(4, edges, 0, 3);

    assert(ek == 3);
    assert(dn == 3);
    assert(ek == dn);

    std::cout << "testSinglePath passed\n";
}

void testNoPath() {
    std::vector<EdgeInput> edges = {
        {0, 1, 5},
        {2, 3, 7}
    };

    long long ek = edmondsKarp(4, edges, 0, 3);
    long long dn = dinic(4, edges, 0, 3);

    assert(ek == 0);
    assert(dn == 0);
    assert(ek == dn);

    std::cout << "testNoPath passed\n";
}

void testParallelEdges() {
    std::vector<EdgeInput> edges = {
        {0, 1, 3},
        {0, 1, 4},
        {1, 2, 10}
    };

    long long ek = edmondsKarp(3, edges, 0, 2);
    long long dn = dinic(3, edges, 0, 2);

    assert(ek == 7);
    assert(dn == 7);
    assert(ek == dn);

    std::cout << "testParallelEdges passed\n";
}

void testDisconnectedSink() {
    std::vector<EdgeInput> edges = {
        {0, 1, 10},
        {1, 2, 5},
        {3, 4, 7}
    };

    long long ek = edmondsKarp(5, edges, 0, 4);
    long long dn = dinic(5, edges, 0, 4);

    assert(ek == 0);
    assert(dn == 0);
    assert(ek == dn);

    std::cout << "testDisconnectedSink passed\n";
}

void testMultipleRoutes() {
    std::vector<EdgeInput> edges = {
        {0, 1, 10},
        {0, 2, 10},
        {1, 3, 4},
        {1, 4, 8},
        {1, 2, 2},
        {2, 4, 9},
        {4, 3, 6},
        {3, 5, 10},
        {4, 5, 10}
    };

    long long ek = edmondsKarp(6, edges, 0, 5);
    long long dn = dinic(6, edges, 0, 5);

    assert(ek == 19);
    assert(dn == 19);
    assert(ek == dn);

    std::cout << "testMultipleRoutes passed\n";
}

int main() {
    testSimpleGraph();
    testSinglePath();
    testNoPath();
    testParallelEdges();
    testDisconnectedSink();
    testMultipleRoutes();

    std::cout << "All max-flow tests passed.\n";

    return 0;
}