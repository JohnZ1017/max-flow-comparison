#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
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

    const std::vector<int> edgeMultipliers = {
        3,   // sparse
        8,   // medium
        15   // dense
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
        << "density,"
        << "seed,"
        << "edmonds_karp_us,"
        << "dinic_us,"
        << "max_flow\n";

    for (int vertices : vertexCounts) {

        for (int multiplier : edgeMultipliers) {

            int edges = vertices * multiplier;

            std::string density;

            if (multiplier == 3) {
                density = "sparse";
            }
            else if (multiplier == 8) {
                density = "medium";
            }
            else {
                density = "dense";
            }

            for (int repetition = 0;
                 repetition < repetitions;
                 ++repetition) {

                unsigned int seed =
                    static_cast<unsigned int>(
                        vertices * 10000
                        + multiplier * 100
                        + repetition
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
                        << " vertices, "
                        << density
                        << " graph, seed "
                        << seed
                        << '\n';

                    return 1;
                }

                output
                    << vertices << ','
                    << edges << ','
                    << density << ','
                    << seed << ','
                    << ekTime << ','
                    << dinicTime << ','
                    << ekFlow << '\n';

                std::cout
                    << "Completed: "
                    << vertices
                    << " vertices, "
                    << density
                    << ", repetition "
                    << repetition + 1
                    << '/'
                    << repetitions
                    << '\n';
            }
        }
    }

    std::cout
        << "Benchmark complete. Results written to "
        << "results/runtime_results.csv\n";

    return 0;
}