#include <chrono>
#include <iostream>
#include <vector>

#include "../src/edmonds_karp.h"
#include "../src/dinic.h"
#include "graph_generator.h"

int main() {
    const int vertices = 100;
    const int edges = 300;
    const long long maxCapacity = 100;
    const unsigned int seed = 42;

    std::vector<EdgeInput> graph = generateRandomGraph(
        vertices,
        edges,
        maxCapacity,
        seed
    );

    const int source = 0;
    const int sink = vertices - 1;

    auto startEK = std::chrono::steady_clock::now();

    long long ekFlow = edmondsKarp(
        vertices,
        graph,
        source,
        sink
    );

    auto endEK = std::chrono::steady_clock::now();

    auto startDinic = std::chrono::steady_clock::now();

    long long dinicFlow = dinic(
        vertices,
        graph,
        source,
        sink
    );

    auto endDinic = std::chrono::steady_clock::now();

    auto ekTime =
        std::chrono::duration_cast<std::chrono::microseconds>(
            endEK - startEK
        ).count();

    auto dinicTime =
        std::chrono::duration_cast<std::chrono::microseconds>(
            endDinic - startDinic
        ).count();

    std::cout << "Vertices: " << vertices << '\n';
    std::cout << "Edges: " << edges << '\n';

    std::cout << "Edmonds-Karp flow: "
              << ekFlow
              << '\n';

    std::cout << "Dinic flow: "
              << dinicFlow
              << '\n';

    std::cout << "Edmonds-Karp time: "
              << ekTime
              << " microseconds\n";

    std::cout << "Dinic time: "
              << dinicTime
              << " microseconds\n";

    if (ekFlow != dinicFlow) {
        std::cerr << "ERROR: Algorithms returned different max-flow values!\n";
        return 1;
    }

    std::cout << "Results match.\n";

    return 0;
}