#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "../src/edmonds_karp.h"
#include "../src/dinic.h"
#include "graph_generator.h"

long long timeEdmondsKarp(
    int vertices,
    const std::vector<EdgeInput>& graph,
    int source,
    int sink,
    long long& flow
) {
    auto start = std::chrono::steady_clock::now();

    flow = edmondsKarp(
        vertices,
        graph,
        source,
        sink
    );

    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(
        end - start
    ).count();
}

long long timeDinic(
    int vertices,
    const std::vector<EdgeInput>& graph,
    int source,
    int sink,
    long long& flow
) {
    auto start = std::chrono::steady_clock::now();

    flow = dinic(
        vertices,
        graph,
        source,
        sink
    );

    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(
        end - start
    ).count();
}

int main() {
    const std::vector<int> vertexCounts = {
        50,
        100,
        200,
        400
    };

    const int repetitions = 5;
    const long long maxCapacity = 100;

    std::ofstream output("results/runtime_results.csv");

    if (!output.is_open()) {
        std::cerr << "Could not open results/runtime_results.csv\n";
        return 1;
    }

    output
        << "vertices,"
        << "edges,"
        << "seed,"
        << "edmonds_karp_us,"
        << "dinic_us,"
        << "max_flow\n";

    for (int vertices : vertexCounts) {
        int edges = vertices * 3;

        for (int repetition = 0;
             repetition < repetitions;
             ++repetition) {

            unsigned int seed =
                static_cast<unsigned int>(
                    vertices * 1000 + repetition
                );

            std::vector<EdgeInput> graph =
                generateRandomGraph(
                    vertices,
                    edges,
                    maxCapacity,
                    seed
                );

            int source = 0;
            int sink = vertices - 1;

            long long ekFlow = 0;
            long long dinicFlow = 0;

            long long ekTime = timeEdmondsKarp(
                vertices,
                graph,
                source,
                sink,
                ekFlow
            );

            long long dinicTime = timeDinic(
                vertices,
                graph,
                source,
                sink,
                dinicFlow
            );

            if (ekFlow != dinicFlow) {
                std::cerr
                    << "ERROR: Flow mismatch for "
                    << vertices
                    << " vertices, seed "
                    << seed
                    << '\n';

                return 1;
            }

            output
                << vertices << ','
                << edges << ','
                << seed << ','
                << ekTime << ','
                << dinicTime << ','
                << ekFlow << '\n';

            std::cout
                << "Completed: "
                << vertices
                << " vertices, repetition "
                << repetition + 1
                << '/'
                << repetitions
                << '\n';
        }
    }

    std::cout
        << "Benchmark complete. Results written to "
        << "results/runtime_results.csv\n";

    return 0;
}