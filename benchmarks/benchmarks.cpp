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
    long long& flow,
    EdmondsKarpStats& stats
) {
    auto start = std::chrono::steady_clock::now();

    flow = edmondsKarp(
        vertices,
        graph,
        source,
        sink,
        &stats
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
    long long& flow,
    DinicStats& stats
) {
    auto start = std::chrono::steady_clock::now();

    flow = dinic(
        vertices,
        graph,
        source,
        sink,
        &stats
    );

    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(
        end - start
    ).count();
}

bool runBenchmark(
    std::ofstream& output,
    const std::string& family,
    int vertices,
    int edges,
    unsigned int seed,
    const std::vector<EdgeInput>& graph
) {
    int source = 0;
    int sink = vertices - 1;

    long long ekFlow = 0;
    long long dinicFlow = 0;

    EdmondsKarpStats ekStats;
    DinicStats dinicStats;

    long long ekTime = timeEdmondsKarp(
        vertices,
        graph,
        source,
        sink,
        ekFlow,
        ekStats
    );

    long long dinicTime = timeDinic(
        vertices,
        graph,
        source,
        sink,
        dinicFlow,
        dinicStats
    );

    if (ekFlow != dinicFlow) {
        std::cerr
            << "ERROR: Flow mismatch for "
            << family
            << ", "
            << vertices
            << " vertices, seed "
            << seed
            << '\n';

        return false;
    }

    output
        << family << ','
        << vertices << ','
        << edges << ','
        << seed << ','
        << ekTime << ','
        << dinicTime << ','
        << ekFlow << ','
        << ekStats.bfsSearches << ','
        << ekStats.augmentations << ','
        << dinicStats.bfsPhases << ','
        << dinicStats.blockingFlowPhases << ','
        << dinicStats.successfulPushes
        << '\n';

    return true;
}

int main() {
    const std::vector<int> vertexCounts = {
        50,
        100,
        200,
        400
    };

    const std::vector<int> edgeMultipliers = {
        3,
        8,
        15
    };

    const int repetitions = 5;
    const long long maxCapacity = 100;

    std::ofstream output("results/runtime_results.csv");

    if (!output.is_open()) {
        std::cerr
            << "Could not open results/runtime_results.csv\n";
        return 1;
    }

    output
        << "family,"
        << "vertices,"
        << "edges,"
        << "seed,"
        << "edmonds_karp_us,"
        << "dinic_us,"
        << "max_flow,"
        << "ek_bfs_searches,"
        << "ek_augmentations,"
        << "dinic_bfs_phases,"
        << "dinic_blocking_phases,"
        << "dinic_successful_pushes\n";

    // -------------------------------------------------
    // Random sparse, medium and dense graphs
    // -------------------------------------------------

    for (int vertices : vertexCounts) {

        for (int multiplier : edgeMultipliers) {

            std::string family;

            if (multiplier == 3) {
                family = "random_sparse";
            }
            else if (multiplier == 8) {
                family = "random_medium";
            }
            else {
                family = "random_dense";
            }

            int edges = vertices * multiplier;

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

                if (!runBenchmark(
                        output,
                        family,
                        vertices,
                        edges,
                        seed,
                        graph
                    )) {
                    return 1;
                }

                std::cout
                    << "Completed: "
                    << family
                    << ", "
                    << vertices
                    << " vertices, repetition "
                    << repetition + 1
                    << '/'
                    << repetitions
                    << '\n';
            }
        }
    }

    // -------------------------------------------------
    // Unit-capacity graphs
    // -------------------------------------------------

    for (int vertices : vertexCounts) {

        int edges = vertices * 8;

        for (int repetition = 0;
             repetition < repetitions;
             ++repetition) {

            unsigned int seed =
                static_cast<unsigned int>(
                    vertices * 20000
                    + repetition
                );

            std::vector<EdgeInput> graph =
                generateUnitCapacityGraph(
                    vertices,
                    edges,
                    seed
                );

            if (!runBenchmark(
                    output,
                    "unit_capacity",
                    vertices,
                    edges,
                    seed,
                    graph
                )) {
                return 1;
            }

            std::cout
                << "Completed: unit_capacity, "
                << vertices
                << " vertices, repetition "
                << repetition + 1
                << '/'
                << repetitions
                << '\n';
        }
    }

    // -------------------------------------------------
    // Layered graphs
    // -------------------------------------------------

    const std::vector<int> layerCounts = {
        4,
        8,
        10,
        20
    };

    const std::vector<int> verticesPerLayer = {
        12,
        12,
        20,
        20
    };

    for (std::size_t i = 0;
         i < layerCounts.size();
         ++i) {

        int layers = layerCounts[i];
        int width = verticesPerLayer[i];

        int vertices =
            layers * width + 2;

        for (int repetition = 0;
             repetition < repetitions;
             ++repetition) {

            unsigned int seed =
                static_cast<unsigned int>(
                    vertices * 30000
                    + repetition
                );

            std::vector<EdgeInput> graph =
                generateLayeredGraph(
                    layers,
                    width,
                    maxCapacity,
                    seed
                );

            int edges =
                static_cast<int>(graph.size());

            if (!runBenchmark(
                    output,
                    "layered",
                    vertices,
                    edges,
                    seed,
                    graph
                )) {
                return 1;
            }

            std::cout
                << "Completed: layered, "
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