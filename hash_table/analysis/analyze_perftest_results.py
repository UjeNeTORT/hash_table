import matplotlib.pyplot as plt
import numpy as np
import pandas
from scipy.stats import norm

def prob_dens (graph_path, arr_ids_funcs):
    plt.xlabel ("find query responce time, TSC");
    plt.ylabel ("probability density")
    plt.grid (linestyle='--')

    for i in arr_ids_funcs:
        x_axis = np.arange (mean_times[i] - 5 * disps[i], mean_times[i] + 5 * disps[i], 1)
        plt.plot (x_axis, norm.pdf (x_axis, mean_times[i], disps[i]), label = names[i] + ": $\\mu$ = " + str (mean_times[i]) + " $\\sigma = $" + str (int (disps[i])))

    plt.savefig (graph_path, bbox_extra_artists=(plt.legend(),), bbox_inches='tight')

    return 0

def bars_graph (graph_path, arr, title):
    plt.figure (figsize=(16, 9))
    plt.title (title)
    plt.xlabel ("time, TSC")
    plt.ylabel ("hash function")
    plt.grid (linestyle='--')
    plt.barh (names, arr)

    plt.savefig (graph_path, bbox_extra_artists=(plt.legend(),), bbox_inches='tight')

TEST_RESULTS_PATH = "hash_table/analysis/results/performance/save2/res.csv"

print ('running')
df       = pandas.read_csv(TEST_RESULTS_PATH)
nparr    = df.to_numpy ()

names      = [i[0] for i in nparr]
mean_times = [i[1] for i in nparr]
disps      = [np.sqrt(i[2]) for i in nparr]
min_times  = [i[3] for i in nparr]
max_times  = [i[4] for i in nparr]

min_x = 0
max_x = 0
for i in range (len (names)):
    min_x = min (min_x, mean_times[i] - 5 * disps[i])
    max_x = max (max_x, mean_times[i] + 5 * disps[i])

bars_graph ("hash_table/analysis/results/graphs_performance/avg_search_time_no_optimize.png", mean_times, 'avg search time')
bars_graph ("hash_table/analysis/results/graphs_performance/max_search_time_no_optimize.png", max_times, 'max search time')
