from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


RESULTS_FILE = Path("results/runtime_results.csv")
PLOTS_DIR = Path("plots")


def load_results():
    if not RESULTS_FILE.exists():
        raise FileNotFoundError(
            f"Could not find {RESULTS_FILE}. "
            "Download runtime_results.csv from the GitHub Actions artifact "
            "and place it inside the results folder."
        )

    return pd.read_csv(RESULTS_FILE)


def prepare_summary(df):
    summary = (
        df.groupby(["family", "vertices"], as_index=False)
        .agg(
            edmonds_karp_median_us=("edmonds_karp_us", "median"),
            dinic_median_us=("dinic_us", "median"),
            ek_bfs_median=("ek_bfs_searches", "median"),
            ek_augmentations_median=("ek_augmentations", "median"),
            dinic_bfs_median=("dinic_bfs_phases", "median"),
            dinic_blocking_median=("dinic_blocking_phases", "median"),
            dinic_pushes_median=("dinic_successful_pushes", "median"),
        )
    )

    summary["dinic_speedup"] = (
        summary["edmonds_karp_median_us"]
        / summary["dinic_median_us"]
    )

    return summary


def plot_runtime_by_family(summary):
    families = summary["family"].unique()

    for family in families:
        family_data = summary[
            summary["family"] == family
        ].sort_values("vertices")

        plt.figure()

        plt.plot(
            family_data["vertices"],
            family_data["edmonds_karp_median_us"],
            marker="o",
            label="Edmonds-Karp"
        )

        plt.plot(
            family_data["vertices"],
            family_data["dinic_median_us"],
            marker="o",
            label="Dinic"
        )

        plt.xlabel("Number of vertices")
        plt.ylabel("Median runtime (microseconds)")
        plt.title(
            f"Runtime vs Graph Size - {family.replace('_', ' ').title()}"
        )

        plt.legend()
        plt.grid(True)

        output_file = PLOTS_DIR / f"runtime_{family}.png"

        plt.tight_layout()
        plt.savefig(output_file, dpi=200)
        plt.close()

        print(f"Created {output_file}")


def plot_speedup(summary):
    plt.figure()

    families = summary["family"].unique()

    for family in families:
        family_data = summary[
            summary["family"] == family
        ].sort_values("vertices")

        plt.plot(
            family_data["vertices"],
            family_data["dinic_speedup"],
            marker="o",
            label=family.replace("_", " ").title()
        )

    plt.axhline(
        y=1,
        linestyle="--"
    )

    plt.xlabel("Number of vertices")
    plt.ylabel("Edmonds-Karp runtime / Dinic runtime")
    plt.title("Dinic Speedup Over Edmonds-Karp")

    plt.legend()
    plt.grid(True)

    output_file = PLOTS_DIR / "dinic_speedup.png"

    plt.tight_layout()
    plt.savefig(output_file, dpi=200)
    plt.close()

    print(f"Created {output_file}")


def plot_layered_operations(summary):
    layered = summary[
        summary["family"] == "layered"
    ].sort_values("vertices")

    if layered.empty:
        print("No layered graph results found.")
        return

    plt.figure()

    plt.plot(
        layered["vertices"],
        layered["ek_bfs_median"],
        marker="o",
        label="Edmonds-Karp BFS searches"
    )

    plt.plot(
        layered["vertices"],
        layered["ek_augmentations_median"],
        marker="o",
        label="Edmonds-Karp augmentations"
    )

    plt.plot(
        layered["vertices"],
        layered["dinic_bfs_median"],
        marker="o",
        label="Dinic BFS phases"
    )

    plt.plot(
        layered["vertices"],
        layered["dinic_blocking_median"],
        marker="o",
        label="Dinic blocking-flow phases"
    )

    plt.xlabel("Number of vertices")
    plt.ylabel("Median operation count")
    plt.title("Internal Operations on Layered Graphs")

    plt.legend()
    plt.grid(True)

    output_file = PLOTS_DIR / "layered_operation_counts.png"

    plt.tight_layout()
    plt.savefig(output_file, dpi=200)
    plt.close()

    print(f"Created {output_file}")


def plot_unit_capacity_runtime(summary):
    unit = summary[
        summary["family"] == "unit_capacity"
    ].sort_values("vertices")

    if unit.empty:
        print("No unit-capacity results found.")
        return

    plt.figure()

    plt.plot(
        unit["vertices"],
        unit["edmonds_karp_median_us"],
        marker="o",
        label="Edmonds-Karp"
    )

    plt.plot(
        unit["vertices"],
        unit["dinic_median_us"],
        marker="o",
        label="Dinic"
    )

    plt.xlabel("Number of vertices")
    plt.ylabel("Median runtime (microseconds)")
    plt.title("Runtime on Unit-Capacity Graphs")

    plt.legend()
    plt.grid(True)

    output_file = PLOTS_DIR / "unit_capacity_runtime.png"

    plt.tight_layout()
    plt.savefig(output_file, dpi=200)
    plt.close()

    print(f"Created {output_file}")


def main():
    PLOTS_DIR.mkdir(exist_ok=True)

    df = load_results()
    summary = prepare_summary(df)

    print("Loaded benchmark results.")
    print()
    print(summary)

    plot_runtime_by_family(summary)
    plot_speedup(summary)
    plot_layered_operations(summary)
    plot_unit_capacity_runtime(summary)

    print()
    print("All plots generated successfully.")


if __name__ == "__main__":
    main()