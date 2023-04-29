import collections
import os
import shutil
import matplotlib
import pandas as pd
import matplotlib.pyplot as plt


def plot_time_series(directory: str):
    """
    Mostra il grafico dei tempi delle varie implementazioni dell'algoritmo di matching

    :param directory:   Directory dove si trovano i file d'interesse per il calcolo del grafico

    :returns:   Il grafico dei tempi
    :rtype:     void
    """

    path = '../build/benchmark/charts/' + directory.split("/")[4]
    d_path = path + '/time_series/'

    if not os.path.exists(path):
        os.makedirs(path)

    if os.path.exists(d_path):
        shutil.rmtree(d_path)
    os.makedirs(d_path)

    if not os.path.exists(directory):
        raise NotADirectoryError(directory, "not exists")
    d = dict()
    for filename in os.listdir(directory):
        f = os.path.join(directory, filename)
        if os.path.isfile(f):
            filename = filename.split(",")
            d[int(filename[0].split(":")[1].split("x")[0])] = int(filename[0].split(":")[1].split("x")[1])

    matplotlib.rcParams['figure.facecolor'] = '#9b86da'
    matplotlib.rcParams['axes.facecolor'] = 'lightblue'
    matplotlib.rcParams['font.family'] = 'serif'
    matplotlib.rcParams['font.size'] = 10
    matplotlib.rcParams['font.weight'] = 'bold'
    matplotlib.rcParams['text.color'] = 'black'
    matplotlib.rcParams['lines.linewidth'] = 1.5

    dict_s = dict()
    dict_sc = dict()
    dict_pc = dict()
    dict_p = dict()

    k = [3, 5, 7, 9, 11]
    b = [8, 16, 32]
    f_name = str()

    for kernel in k:
        for block in b:
            for filename in os.listdir(directory):
                f = os.path.join(directory, filename)
                if os.path.isfile(f) and "kernel: " + str(kernel) + "x" + str(kernel) + ",BlockDim: " + str(
                        block) + "x" + str(block) in f:
                    df = pd.read_csv(f)
                    f_name = filename.split('_')[0].split(',')[1] + ' - ' + filename.split('_')[0].split(',')[2]
                    # el = int(filename.split(" ")[1].split(',')[0].split('x')[0] + (filename.split(" ")[1].split(',')[0].split('x')[1]))

                    el = int(filename.split(" ")[1].split(',')[0].split('x')[0])

                    dict_s[el] = round(float(df.iloc[[0]]['mean']), 4)
                    dict_sc[el] = round(float(df.iloc[[1]]['mean']), 4)
                    dict_pc[el] = round(float(df.iloc[[2]]['mean']), 4)
                    dict_p[el] = round(float(df.iloc[[3]]['mean']), 4)

            dict_s = collections.OrderedDict(sorted(dict_s.items()))
            dfs = pd.DataFrame(dict_s.items(), columns=['size', 'time'])
            dfs.set_index("size", inplace=True)
            dfs.name = 'sequential'

            dict_sc = collections.OrderedDict(sorted(dict_sc.items()))
            dfsc = pd.DataFrame(dict_sc.items(), columns=['size', 'time'])
            dfsc.set_index("size", inplace=True)
            dfsc.name = 'sequentialWithCopy'

            dict_pc = collections.OrderedDict(sorted(dict_pc.items()))
            dfpc = pd.DataFrame(dict_pc.items(), columns=['size', 'time'])
            dfpc.set_index("size", inplace=True)
            dfpc.name = 'parallelWithCopy'

            dict_p = collections.OrderedDict(sorted(dict_p.items()))
            dfp = pd.DataFrame(dict_p.items(), columns=['size', 'time'])
            dfp.set_index("size", inplace=True)
            dfp.name = 'parallel'

            time = [dfs, dfsc, dfpc, dfp]

            fig = plt.figure()

            for frame in time:
                plt.plot(frame['time'], label=frame.name + "\n" + str(frame))

            leg = plt.legend(loc='best', bbox_to_anchor=(0.42, 1),
                             ncol=3, fancybox=True, shadow=True)

            for el in d.keys():
                plt.plot(el, dict_s.get(el), 'ro')
                plt.text(el + 0.1, dict_s.get(el) + 0.1, str(el) + 'x' + str(d.get(el)), fontsize=7)

                plt.plot(el, dict_sc.get(el), 'ro')
                plt.text(el + 0.1, dict_sc.get(el) + 0.1, str(el) + 'x' + str(d.get(el)), fontsize=7)

                plt.plot(el, dict_pc.get(el), 'ro')
                plt.text(el + 0.1, dict_pc.get(el) + 0.1, str(el) + 'x' + str(d.get(el)), fontsize=7)

                plt.plot(el, dict_p.get(el), 'ro')
                plt.text(el + 0.1, dict_p.get(el) + 0.1, str(el) + 'x' + str(d.get(el)), fontsize=7)
                # plt.text(el + 0.1, dict_dfp.get(el) + 0.1, el, fontsize=9)

            plt.title(
                "TIME BENCHMARK → " + f_name.upper(),
                fontsize='20',
                loc='center',
                fontweight='bold',
                style='italic',
                family='monospace')

            plt.xlabel('Size', fontweight='bold', style='italic', fontsize=15)
            plt.ylabel('Time[ms]', style='italic', loc='center', fontweight='bold', fontsize=15)

            plot_backend = matplotlib.get_backend()
            mng = plt.get_current_fig_manager()
            if plot_backend == 'TkAgg':
                mng.resize(*mng.window.maxsize())
            elif plot_backend == 'wxAgg':
                mng.frame.Maximize(True)
            elif plot_backend == 'Qt4Agg':
                mng.window.showMaximized()

            plt.grid()

            fig.set_size_inches((22, 11), forward=False)
            plt.savefig(d_path + f_name + '.png', dpi=500, pad_inches=0, bbox_inches='tight')

            plt.show()

            dict_s.clear()
            dict_p.clear()
            dict_pc.clear()
            dict_sc.clear()

    print('→ Time series chart calculated successfully!\n→ File path: ' + d_path)


if __name__ == '__main__':
    #plot_time_series('../build/benchmark/results/standard_formats')
    print('Time_series chart')