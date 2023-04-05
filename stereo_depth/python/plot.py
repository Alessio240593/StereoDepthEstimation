import os
from typing import List
import pandas as pd
import matplotlib.pyplot as plt
import statistics


def plot_graph_mean(col: str, l: List):
    df = pd.DataFrame(columns=['size', 'time']).set_index('size')

    seq_c_list = list()
    seq_list = list()
    par_c_list = list()
    par_list = list()

    dict_dfs = dict()
    dict_dfsc = dict()
    dict_dfpc = dict()
    dict_dfp = dict()

    for el in l:
        for filename in os.listdir(directory):
            f = os.path.join(directory, filename)
            if os.path.isfile(f) and "matrix: " + str(el) + "x" + str(el) in f:
                print(f, el)
                df = pd.read_csv(f)
                seq_c_list.append((float(df.iloc[[0]][col])))
                seq_list.append((float(df.iloc[[1]][col])))
                par_c_list.append((float(df.iloc[[2]][col])))
                par_list.append((float(df.iloc[[3]][col])))

        dict_dfs[el] = statistics.mean(seq_list)
        dict_dfsc[el] = statistics.mean(seq_c_list)
        dict_dfpc[el] = statistics.mean(par_c_list)
        dict_dfp[el] = statistics.mean(par_list)

    dfs = pd.DataFrame(dict_dfs.items(), columns=['size', 'time'])
    dfs.set_index("size", inplace=True)
    dfs.name = 'sequential'

    dfsc = pd.DataFrame(dict_dfsc.items(), columns=['size', 'time'])
    dfsc.set_index("size", inplace=True)
    dfsc.name = 'sequentialWithCopy'

    dfpc = pd.DataFrame(dict_dfpc.items(), columns=['size', 'time'])
    dfpc.set_index("size", inplace=True)
    dfpc.name = 'parallelWithCopy'

    dfp = pd.DataFrame(dict_dfp.items(), columns=['size', 'time'])
    dfp.set_index("size", inplace=True)
    dfp.name = 'parallel'

    time = [dfs, dfsc, dfpc, dfp]

    for frame in time:
        plt.plot(frame['time'], label=frame.name + "\n" + str(frame))

    leg = plt.legend(loc='upper right', bbox_to_anchor=(0.5, 1.05),
                     ncol=3, fancybox=True, shadow=True)

    for el in l:
        plt.plot(el, dict_dfs.get(el), 'ro')
        plt.text(el + 0.1, dict_dfs.get(el) + 0.1, el, fontsize=9)

        plt.plot(el, dict_dfsc.get(el), 'ro')
        plt.text(el + 0.1, dict_dfsc.get(el) + 0.1, el, fontsize=9)

        plt.plot(el, dict_dfpc.get(el), 'ro')
        plt.text(el + 0.1, dict_dfpc.get(el) + 0.1, el, fontsize=9)

        plt.plot(el, dict_dfp.get(el), 'ro')
        plt.text(el + 0.1, dict_dfp.get(el) + 0.1, el, fontsize=9)

    # manager = plt.get_current_fig_manager()
    # manager.full_screen_toggle()
    plt.title(
        "Sequential-parallel graph with " + col,
        fontsize='large',
        loc='right',
        fontweight='bold',
        style='italic',
        family='monospace')
    plt.xlabel('Size', fontweight='bold', style='italic')
    plt.ylabel('Time', style='italic', loc='bottom', fontweight='bold')
    plt.grid()
    plt.show()


def plot_graph_median(col: str, l: List):
    df = pd.DataFrame(columns=['size', 'time']).set_index('size')

    seq_c_list = list()
    seq_list = list()
    par_c_list = list()
    par_list = list()

    dict_dfs = dict()
    dict_dfsc = dict()
    dict_dfpc = dict()
    dict_dfp = dict()

    for el in l:
        for filename in os.listdir(directory):
            f = os.path.join(directory, filename)
            if os.path.isfile(f) and "matrix: " + str(el) + "x" + str(el) in f:
                print(f, el)
                df = pd.read_csv(f)
                seq_c_list.append((float(df.iloc[[0]][col])))
                seq_list.append((float(df.iloc[[1]][col])))
                par_c_list.append((float(df.iloc[[2]][col])))
                par_list.append((float(df.iloc[[3]][col])))

        dict_dfs[el] = statistics.median(seq_list)
        dict_dfsc[el] = statistics.median(seq_c_list)
        dict_dfpc[el] = statistics.median(par_c_list)
        dict_dfp[el] = statistics.median(par_list)

    dfs = pd.DataFrame(dict_dfs.items(), columns=['size', 'time'])
    dfs.set_index("size", inplace=True)
    dfs.name = 'sequential'

    dfsc = pd.DataFrame(dict_dfsc.items(), columns=['size', 'time'])
    dfsc.set_index("size", inplace=True)
    dfsc.name = 'sequentialWithCopy'

    dfpc = pd.DataFrame(dict_dfpc.items(), columns=['size', 'time'])
    dfpc.set_index("size", inplace=True)
    dfpc.name = 'parallelWithCopy'

    dfp = pd.DataFrame(dict_dfp.items(), columns=['size', 'time'])
    dfp.set_index("size", inplace=True)
    dfp.name = 'parallel'

    time = [dfs, dfsc, dfpc, dfp]

    for frame in time:
        plt.plot(frame['time'], label=frame.name + "\n" + str(frame))

    leg = plt.legend(loc='upper right', bbox_to_anchor=(0.5, 1.05),
                     ncol=3, fancybox=True, shadow=True)

    for el in l:
        plt.plot(el, dict_dfs.get(el), 'ro')
        plt.text(el + 0.1, dict_dfs.get(el) + 0.1, el, fontsize=9)

        plt.plot(el, dict_dfsc.get(el), 'ro')
        plt.text(el + 0.1, dict_dfsc.get(el) + 0.1, el, fontsize=9)

        plt.plot(el, dict_dfpc.get(el), 'ro')
        plt.text(el + 0.1, dict_dfpc.get(el) + 0.1, el, fontsize=9)

        plt.plot(el, dict_dfp.get(el), 'ro')
        plt.text(el + 0.1, dict_dfp.get(el) + 0.1, el, fontsize=9)

    # manager = plt.get_current_fig_manager()
    # manager.full_screen_toggle()
    plt.title(
        "Sequential-parallel graph with " + col,
        fontsize='large',
        loc='right',
        fontweight='bold',
        style='italic',
        family='monospace')
    plt.grid()
    plt.show()


directory = "../build/benchmark/results"

if not os.path.exists(directory):
    raise NotADirectoryError(directory, "not exists")
l = set()
for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    if os.path.isfile(f):
        filename = filename.split(",")
        l.add(int(filename[0].split(":")[1].split("x")[1]))

l = list(l)
l.sort()

plot_graph_mean('mean', l)
#plot_graph_mean('median', l)
#plot_graph_median('mean', l)
plot_graph_median('median', l)
