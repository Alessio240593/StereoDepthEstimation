import os
import shutil
from typing import List
import matplotlib
import pandas as pd
from matplotlib import pyplot as plt


def boxplot(i: int, j: int, ax, data: List):
    """
    Calcola il boxplot per un dato subplot (i, j)

    :param i:       Indice riga per il subplot
    :param j:       Indice colonna per il subplot
    :param ax:      Riferimento agli assi del subplot
    :param data:    Dati per il grafico

    :return:        Disegna i boxplot per il relativo subplot (i, j)
    :rtype:         void
    """

    # Creating axes instance
    bp = ax[i, j].boxplot(data, patch_artist=True,
                          vert=0, meanprops={'markerfacecolor': 'red', 'markeredgecolor': 'black', 'markersize': '12'}, showfliers=True)

    colors = ['#0000FF', '#00FF00',
              '#FFFF00', '#FF00FF']

    for patch, color in zip(bp['boxes'], colors):
        patch.set_facecolor(color)

    # changing color and linewidth of
    # whiskers
    for whisker in bp['whiskers']:
        whisker.set(color='#8B008B',
                    linewidth=1.5,
                    linestyle=":")

    # changing color and linewidth of
    # caps
    for cap in bp['caps']:
        cap.set(color='#8B008B',
                linewidth=2)

    # changing color and linewidth of
    # medians
    for median in bp['medians']:
        median.set(color='red',
                   linewidth=3)

    # changing style of fliers
    for flier in bp['fliers']:
        flier.set(marker='D',
                  color='#e7298a',
                  alpha=0.5)

    ax[i, j].set_xlabel("Time[ms]", fontweight='bold', style='italic', fontsize=13)

    # Removing top axes and right axes
    # ticks
    ax[i, j].get_xaxis().tick_bottom()
    ax[i, j].get_yaxis().tick_left()

    ax[i, j].grid()


def time_series_boxplot(directory: str):
    """
    Calcola il grafico dei tempi con boxplot

    :param directory:   Directory dove si trovano i file d'interesse per il calcolo del grafico

    :return:            Il grafico dei tempi con boxplot
    :rtype:             void
    """

    path = '../build/benchmark/charts/' + directory.split("/")[4]
    d_path = path + '/time_series_boxplot/'

    if not os.path.exists(path):
        os.makedirs(path)

    if os.path.exists(d_path):
        shutil.rmtree(d_path)
    os.makedirs(d_path)


    # Main
    matplotlib.rcParams['figure.facecolor'] = '#9b86da'
    matplotlib.rcParams['axes.facecolor'] = 'lightblue'
    matplotlib.rcParams['font.family'] = 'serif'
    matplotlib.rcParams['font.size'] = 10
    matplotlib.rcParams['font.weight'] = 'bold'
    matplotlib.rcParams['text.color'] = 'black'
    matplotlib.rcParams['lines.linewidth'] = 1.5

    if not os.path.exists(directory):
        raise NotADirectoryError(directory, "not exists")
    d = dict()
    for filename in os.listdir(directory):
        f = os.path.join(directory, filename)
        if os.path.isfile(f):
            filename = filename.split(",")
            d[int(filename[0].split(":")[1].split("x")[0])] = int(filename[0].split(":")[1].split("x")[1])

    seq_c_list = list()
    seq_list = list()
    par_c_list = list()
    par_list = list()

    dim = 0

    for el in d.keys():
        for filename in os.listdir(directory):
            f = os.path.join(directory, filename)
            if os.path.isdir(f) and "matrix: " + str(el) + "x" + str(
                    d.get(el)) + ",kernel: 3x3,BlockDim: 16x16_values" in f:
                dim = dim + 1

    r = 2
    c = 2
    keys = sorted([*d])
    keys_1 = keys[0:2]
    keys_2 = keys[2:4]
    i = 0
    j = 0
    k = [3, 5, 7, 9, 11]
    b = [8, 16, 32]
    f_name = str()

    for kernel in k:
        for block in b:
            fig, ax = plt.subplots(r, c)
            plt.subplots_adjust(hspace=0.5)
            for el in keys_1:
                for filename in os.listdir(directory):
                    f = os.path.join(directory, filename)
                    if os.path.isdir(f) and 'matrix: ' + str(el) + 'x' + str(d.get(el)) + ",kernel: " + str(
                            kernel) + "x" + str(kernel) + ",BlockDim: " + str(
                        block) + "x" + str(block) in f:
                        for file in os.listdir(f):
                            df = pd.read_csv(os.path.join(f, file))

                            f_name = filename.split('_')[0].split(',')[1] + ' - ' + filename.split('_')[0].split(',')[2]

                            if "cpu_seq_c" in file:
                                seq_c_list = list([float(i) for i in df.values])
                            elif "cpu_seq" in file:
                                seq_list = list([float(i) for i in df.values])
                            elif "gpu_par_c" in file:
                                par_c_list = list([float(i) for i in df.values])
                            else:
                                par_list = list([float(i) for i in df.values])

                        #break

                data_s = [seq_c_list, seq_list]
                data_p = [par_c_list, par_list]

                boxplot(i, j, ax, data_p)
                # x-axis labels
                ax[i, j].set_yticklabels(['par with copy', 'par'])
                ax[i, j].set_title(str(keys_1[i]) + ' x ' + str(d.get(keys_1[i])), fontsize=13,
                                   fontweight="bold", color="black")

                boxplot(i, j + 1, ax, data_s)
                # x-axis labels
                ax[i, j + 1].set_yticklabels(['seq with copy', 'seq'])
                ax[i, j + 1].set_title(str(keys_1[i]) + ' x ' + str(d.get(keys_1[i])), fontsize=13,
                                       fontweight="bold", color="black")

                i = i + 1

            plt.suptitle(
                "TIME SERIES BOXPLOT → " + f_name.upper(),
                fontsize='20',
                fontweight='bold',
                style='italic',
                family='monospace')

            plot_backend = matplotlib.get_backend()
            mng = plt.get_current_fig_manager()
            if plot_backend == 'TkAgg':
                mng.resize(*mng.window.maxsize())
            elif plot_backend == 'wxAgg':
                mng.frame.Maximize(True)
            elif plot_backend == 'Qt4Agg':
                mng.window.showMaximized()

            fig.set_size_inches((22, 11), forward=False)

            plt.savefig(d_path + f_name + '__1' + '.png', dpi=500, pad_inches=0, bbox_inches='tight')

            plt.show()
            i = 0

            fig, ax = plt.subplots(r - 1, c)

            for el in keys_2:
                for filename in os.listdir(directory):
                    f = os.path.join(directory, filename)
                    if os.path.isdir(f) and 'matrix: ' + str(el) + 'x' + str(d.get(el)) + ",kernel: " + str(
                            kernel) + "x" + str(kernel) + ",BlockDim: " + str(
                        block) + "x" + str(block) in f:
                        for file in os.listdir(f):
                            df = pd.read_csv(os.path.join(f, file))

                            f_name = filename.split('_')[0].split(',')[1] + ' - ' + filename.split('_')[0].split(',')[2]

                            if "cpu_seq_c" in file:
                                seq_c_list = list([float(i) for i in df.values])
                            elif "cpu_seq" in file:
                                seq_list = list([float(i) for i in df.values])
                            elif "gpu_par_c" in file:
                                par_c_list = list([float(i) for i in df.values])
                            else:
                                par_list = list([float(i) for i in df.values])

                        break

                data_s = [seq_c_list, seq_list]
                data_p = [par_c_list, par_list]

                boxplot(i, j, ax, data_p)
                # x-axis labels
                ax[i, j].set_yticklabels(['par with copy', 'par'])
                ax[i, j].set_title(str(keys_2[i]) + ' x ' + str(d.get(keys_2[i])), fontsize=13,
                                   fontweight="bold", color="black")

                boxplot(i, j + 1, ax, data_s)
                # x-axis labels
                ax[i, j + 1].set_yticklabels(['seq with copy', 'seq' ])
                ax[i, j + 1].set_title(str(keys_2[i]) + ' x ' + str(d.get(keys_2[i])), fontsize=13,
                                       fontweight="bold", color="black")

                i = i + 1

            plt.suptitle(
                "TIME SERIES BOXPLOT → " + f_name.upper(),
                fontsize='20',
                fontweight='bold',
                style='italic',
                family='monospace')

            plot_backend = matplotlib.get_backend()
            mng = plt.get_current_fig_manager()
            if plot_backend == 'TkAgg':
                mng.resize(*mng.window.maxsize())
            elif plot_backend == 'wxAgg':
                mng.frame.Maximize(True)
            elif plot_backend == 'Qt4Agg':
                mng.window.showMaximized()

            fig.set_size_inches((22, 11), forward=False)

            plt.savefig(d_path + f_name + '__2' + '.png', dpi=500, pad_inches=0, bbox_inches='tight')

            plt.show()
            i = 0

    print('→ Time series boxplot chart calculated successfully!\n→ File path: ' + d_path)


if __name__ == '__main__':
    #time_series_boxplot('../build/benchmark/results/32-512_formats')
    print('Time_series_boxplot_standard_formats chart')