import matplotlib.pyplot as plt
import numpy as np
import pandas

def draw_load_hist (hf_name, style):
    fpath = "hash_table/analysis/results/csv/" + hf_name + ".csv"
    df       = pandas.read_csv(fpath)
    nparr    = df.to_numpy ()
    ids      = [i[0] for i in nparr]
    l_length = [i[1] for i in nparr]

    disp = np.var (l_length)
    lf   = sum (l_length) / len (l_length)

    plt.subplot ()
    plt.show ()
    plt.style.reload_library()
    plt.style.use(style)
    plt.title ("load historgram for " + hf_name + " hash function")
    plt.xlabel ("id")
    plt.ylabel ("list length")
    plt.grid (linestyle= ":")
    plt.bar (ids, l_length, width=10, color = "blue", label = "load_factor = " + str(round(lf, 2)) + " variance = " + str(round(disp, 2)))
    plt.savefig ("hash_table/analysis/results/graphs/" + hf_name + "_" + style + ".png", bbox_extra_artists=(plt.legend(),), bbox_inches='tight')

    plt.legend ()

    return disp, lf

draw_load_hist ("retzero_hf", "ggplot")
draw_load_hist ("ascii_first_hf", "ggplot")
draw_load_hist ("wd_len_hf", "ggplot")
draw_load_hist ("ctrl_sum_hf", "ggplot")
draw_load_hist ("sum_by_len_hf", "ggplot")
draw_load_hist ("rol", "ggplot")
draw_load_hist ("crc32_hf", "ggplot")

draw_load_hist ("retzero_hf", "dark_background")
draw_load_hist ("ascii_first_hf", "dark_background")
draw_load_hist ("wd_len_hf", "dark_background")
draw_load_hist ("ctrl_sum_hf", "dark_background")
draw_load_hist ("sum_by_len_hf", "dark_background")
draw_load_hist ("rol", "dark_background")
draw_load_hist ("crc32_hf", "dark_background")