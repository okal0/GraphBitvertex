import matplotlib.pyplot as plt
import pandas as pd

def read_execution_times(file_path):
    data = []
    with open(file_path, 'r') as file:
        lines = file.readlines()
        graph_size, num_edges = None, None
        for line in lines:
            line = line.strip()
            if "Graph Size" in line:
                parts = line.split(",")
                try:
                    graph_size = int(parts[0].split(":")[1].strip())
                    num_edges = int(parts[1].split(":")[1].strip())
                except IndexError:
                    print(f"Skipping line: {line}")
                    continue
            elif "DFS" in line or "BFS" in line or "TPS" in line:
                parts = line.split(",")
                if len(parts) >= 3:
                    try:
                        algorithm = parts[0].strip()
                        bitvertex_time = float(parts[1].strip())
                        standard_time = float(parts[2].strip())
                        data.append([graph_size, num_edges, algorithm, bitvertex_time, standard_time])
                    except IndexError:
                        print(f"Skipping line: {line}")
                        continue
    return pd.DataFrame(data, columns=["Graph Size", "Edges", "Algorithm", "BitVertex (ms)", "Standard (ms)"])

def plot_execution_times(file_path, algorithm_name, output_file):
    df = read_execution_times(file_path)
    plt.figure(figsize=(10, 6))
    bar_width = 0.35
    index = df["Graph Size"].unique()
    r1 = range(len(index))
    r2 = [x + bar_width for x in r1]
    
    for label in df["Algorithm"].unique():
        df_algo = df[df["Algorithm"] == label]
        bars_bitvertex = plt.bar(r1, df_algo["BitVertex (ms)"], width=bar_width, label=f'BitVertex {label}')
        bars_standard = plt.bar(r2, df_algo["Standard (ms)"], width=bar_width, label=f'Standard {label}')
        
        for bar in bars_bitvertex:
            plt.text(bar.get_x() + bar.get_width() / 2, bar.get_height(), f'{bar.get_height():.3f}', 
                     ha='center', va='bottom', fontsize=9)
        for bar in bars_standard:
            plt.text(bar.get_x() + bar.get_width() / 2, bar.get_height(), f'{bar.get_height():.3f}', 
                     ha='center', va='bottom', fontsize=9)
    
    for i, size in enumerate(index):
        edge_size = df[df["Graph Size"] == size]["Edges"].iloc[0]
        plt.text(i, -0.3, f'Edges: {edge_size}', ha='center', va='top', fontsize=9)
    
    plt.title(f"{algorithm_name} Execution Times Comparison")
    plt.xlabel("Graph Size")
    plt.ylabel("Execution Time (ms)")
    plt.xticks(r1, index)
    plt.legend()
    plt.grid(True, axis='y')
    plt.tight_layout()
    plt.savefig(output_file)
    plt.close()


plot_execution_times('bfs_execution_times.txt', 'BFS', 'bfs_execution_times.png')
plot_execution_times('dfs_execution_times.txt', 'DFS', 'dfs_execution_times.png')
plot_execution_times('topological_execution_times.txt', 'Topological Sort', 'topological_execution_times.png')

print("Execution time plots saved.")
