# Max-Flow Algorithm Comparison

Advanced Algorithms Programming Assignment 2.

This project compares two maximum-flow algorithms:

- Edmonds-Karp
- Dinic's Algorithm

The repository includes correctness tests, graph generators, automated benchmarks and plotting tools used for the experimental comparison.

## Correctness Tests

The shared test suite checks both Edmonds-Karp and Dinic on the same graphs and verifies that they return the expected maximum-flow values.

Tests are automatically run through GitHub Actions whenever code is pushed to the repository.

To view the results:

1. Open the repository on GitHub.
2. Select the **Actions** tab.
3. Open the latest **Run C++ Tests** workflow.
4. Open the test job to view the output.

If `g++` is installed locally, the tests can also be compiled with:

```bash
g++ -std=c++17 tests/tests.cpp src/edmonds_karp.cpp src/dinic.cpp -o tests_runner
./tests_runner
```

## Running the Benchmarks

Benchmarks are run through a manually triggered GitHub Actions workflow.

To run them:

1. Open the **Actions** tab on GitHub.
2. Select **Run Benchmarks**.
3. Select **Run workflow**.
4. Wait for the benchmark job to finish.
5. Download the `benchmark-results` artifact.

The artifact contains:

```text
runtime_results.csv
```

The benchmark compares both algorithms on:

- random sparse graphs
- random medium-density graphs
- random dense graphs
- layered graphs
- unit-capacity graphs

Each configuration is repeated five times using fixed random seeds.

The CSV records:

- graph family
- number of vertices
- number of edges
- random seed
- Edmonds-Karp runtime
- Dinic runtime
- maximum-flow value
- Edmonds-Karp BFS searches
- Edmonds-Karp augmentations
- Dinic BFS phases
- Dinic blocking-flow phases
- Dinic successful flow pushes

## Generating Plots

Place the downloaded benchmark CSV at:

```text
results/runtime_results.csv
```

The plotting script requires Python with `pandas` and `matplotlib`.

Install the required packages:

```bash
python -m pip install pandas matplotlib
```

Then run:

```bash
python plots/generate_plots.py
```

The generated figures will be saved in the `plots/` folder.

## Requirements

For local execution:

- C++17 compatible compiler
- Python 3
- pandas
- matplotlib

GitHub Actions can be used to run the C++ tests and benchmarks without a local C++ compiler.
