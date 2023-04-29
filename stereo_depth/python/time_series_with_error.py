import collections
import os
import shutil
import matplotlib
import matplotlib.pyplot as plt
import pandas as pd


def plot_time_series_with_error(directory: str):
    """
    Mostra il grafico dei tempi con il tasso d'errore delle varie implementazioni dell'algoritmo di matching

    :param directory:   Directory dove si trovano i file d'interesse per il calcolo del grafico

    :returns:   Il grafico dei tempi con il tasso d'errore
    :rtype:     void
    """

    path = '../build/benchmark/charts/' + directory.split("/")[4]
    d_path = path + '/time_series_with_error/'

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
    matplotlib.rcParams['lines.linewidth'] = 1

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

                    el = int(filename.split(" ")[1].split(',')[0].split('x')[0])

                    dict_s[el] = ((round(float(df.iloc[[0]]['mean']), 4)), (round(float(df.iloc[[0]]['std']), 4)))
                    dict_sc[el] = ((round(float(df.iloc[[1]]['mean']), 4)), (round(float(df.iloc[[1]]['std']), 4)))
                    dict_pc[el] = ((round(float(df.iloc[[2]]['mean']), 4)), (round(float(df.iloc[[2]]['std']), 4)))
                    dict_p[el] = ((round(float(df.iloc[[3]]['mean']), 4)), (round(float(df.iloc[[3]]['std']), 4)))

            std_s = dict()
            std_sc = dict()
            std_p = dict()
            std_pc = dict()

            for el in sorted(dict_s.keys()):
                std_s[el] = float(str(dict_s.get(el)[1]))
                dict_s[el] = float(str(dict_s.get(el)[0]))

            dict_s = collections.OrderedDict(sorted(dict_s.items()))

            dfs = pd.DataFrame(dict_s.items(), columns=['size', 'time'])
            dfs.set_index("size", inplace=True)
            dfs.name = 'sequential'

            dfs.sort_values(by=['size'], inplace=True)
            sorted(std_s)

            for el in sorted(dict_sc.keys()):
                std_sc[el] = float(str(dict_sc.get(el)[1]))
                dict_sc[el] = float(str(dict_sc.get(el)[0]))

            dict_sc = collections.OrderedDict(sorted(dict_sc.items()))

            dfsc = pd.DataFrame(dict_sc.items(), columns=['size', 'time'])
            dfsc.set_index("size", inplace=True)
            dfsc.name = 'sequentialWithCopy'

            dfsc.sort_values(by=['size'], inplace=True)
            sorted(std_sc)

            for el in sorted(dict_pc.keys()):
                std_pc[el] = float(str(dict_pc.get(el)[1]))
                dict_pc[el] = float(str(dict_pc.get(el)[0]))

            dict_pc = collections.OrderedDict(sorted(dict_pc.items()))

            dfpc = pd.DataFrame(dict_pc.items(), columns=['size', 'time'])
            dfpc.set_index("size", inplace=True)
            dfpc.name = 'parallelWithCopy'

            dfpc.sort_values(by=['size'], inplace=True)
            sorted(std_pc)

            for el in sorted(dict_p.keys()):
                std_p[el] = float(str(dict_p.get(el)[1]))
                dict_p[el] = float(str(dict_p.get(el)[0]))

            dict_p = collections.OrderedDict(sorted(dict_p.items()))

            dfp = pd.DataFrame(dict_p.items(), columns=['size', 'time'])
            dfp.set_index("size", inplace=True)
            dfp.name = 'parallel'

            dfp.sort_values(by=['size'], inplace=True)
            sorted(std_p)

            time = [dfs, dfsc, dfpc, dfp]
            std = [[i for i in sorted(std_s.values())], [i for i in sorted(std_sc.values())],
                   [i for i in sorted(std_pc.values())], [i for i in sorted(std_p.values())]]
            colors = ['blue', 'orange', 'green', 'red']

            fig = plt.figure()

            sub = list()
            sum = list()
            pos = 0

            for frame, color in zip(time, colors):
                for op1, op2 in zip(frame['time'].values, std[pos]):
                    sub.append(op1 - op2)
                    sum.append(op1 + op2)
                plt.plot(frame['time'], label=frame.name + "\n" + str(frame))
                plt.fill_between(sorted(d.keys()), sub, sum, color=color, alpha=0.3)
                sum.clear()
                sub.clear()
                pos = pos + 1

            leg = plt.legend(loc='best', bbox_to_anchor=(0.42, 1),
                             ncol=3, fancybox=True, shadow=True)

            plot_backend = matplotlib.get_backend()
            mng = plt.get_current_fig_manager()
            if plot_backend == 'TkAgg':
                mng.resize(*mng.window.maxsize())
            elif plot_backend == 'wxAgg':
                mng.frame.Maximize(True)
            elif plot_backend == 'Qt4Agg':
                mng.window.showMaximized()

            plt.title(
                "TIME BENCHMARK WITH ERROR → " + f_name.upper(),
                fontsize='20',
                loc='center',
                fontweight='bold',
                style='italic',
                family='monospace')

            plt.grid()

            fig.set_size_inches((22, 11), forward=False)

            plt.savefig(d_path + f_name + '.png', dpi=500, pad_inches=0, bbox_inches='tight')

            plt.show()

            dict_s.clear()
            dict_sc.clear()
            dict_pc.clear()
            dict_p.clear()

            std_s.clear()
            std_sc.clear()
            std_p.clear()
            std_pc.clear()

    print('→ Time series with error chart calculated successfully!\n→ File path: ' + d_path)


if __name__ == '__main__':
    #plot_time_series_with_error('../build/benchmark/results/32-512_formats')
    print('Time_series_with_error chart')