import collections
import os
import shutil
import matplotlib
import pandas as pd
import matplotlib.pyplot as plt


def speedup_bar_chart(directory: str):
    """
    Mostra il grafico degli speedUp tra le varie implementazioni dell'algoritmo di matching

    :param directory:   Directory dove si trovano i file d'interesse per il calcolo del grafico

    :returns:   Il grafico degli speedUp
    :rtype:     void
    """

    path = '../build/benchmark/charts/' + directory.split("/")[4]
    d_path = path + '/speed_up_bar_chart/'

    if not os.path.exists(path):
        os.makedirs(path)

    if os.path.exists(d_path):
        shutil.rmtree(d_path)
    os.makedirs(d_path)

    matplotlib.rcParams['figure.facecolor'] = 'lightblue'
    matplotlib.rcParams['axes.facecolor'] = '#ecc1e1'
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

    dict_seq_c_to_par_c = dict()
    dict_seq_c_to_par = dict()
    dict_seq_to_par_c = dict()
    dict_seq_to_par = dict()

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

                    dict_seq_c_to_par_c[el] = float(str(df.iloc[[6]]['mean']).split('x')[0].split(' ')[4])
                    dict_seq_c_to_par[el] = float(str(df.iloc[[7]]['mean']).split('x')[0].split(' ')[4])
                    dict_seq_to_par_c[el] = float(str(df.iloc[[8]]['mean']).split('x')[0].split(' ')[4])
                    dict_seq_to_par[el] = float(str(df.iloc[[9]]['mean']).split('x')[0].split(' ')[4])

            dict_seq_c_to_par_c = collections.OrderedDict(sorted(dict_seq_c_to_par_c.items()))
            dfs = pd.DataFrame(dict_seq_c_to_par_c.items(), columns=['size', 'time'])
            dfs.set_index("size", inplace=True)
            dfs.name = 'seq_c_to_par_c'

            dict_seq_c_to_par = collections.OrderedDict(sorted(dict_seq_c_to_par.items()))
            dfsc = pd.DataFrame(dict_seq_c_to_par.items(), columns=['size', 'time'])
            dfsc.set_index("size", inplace=True)
            dfsc.name = 'seq_c_to_par'

            dict_seq_to_par_c = collections.OrderedDict(sorted(dict_seq_to_par_c.items()))
            dfpc = pd.DataFrame(dict_seq_to_par_c.items(), columns=['size', 'time'])
            dfpc.set_index("size", inplace=True)
            dfpc.name = 'seq_to_par_c'

            dict_seq_to_par = collections.OrderedDict(sorted(dict_seq_to_par.items()))
            dfp = pd.DataFrame(dict_seq_to_par.items(), columns=['size', 'time'])
            dfp.set_index("size", inplace=True)
            dfp.name = 'seq_to_par'

            fig, ax = plt.subplots(nrows=2, ncols=2)
            # fig.tight_layout()

            dfs_ax = dfs.plot(kind='bar', figsize=(12, 8), legend=False, colormap='Paired',
                              ax=ax[0, 0], edgecolor="#9b86da", rot=0)

            ax[0, 0].set_xlabel('Size', fontsize=13,
                                fontweight="bold", labelpad=5, color="#9b86da")

            ax[0, 0].set_ylabel('SpeedUp[x]', fontsize=13,
                                fontweight="bold", labelpad=5, color="#9b86da")

            ax[0, 0].set_title('Sequential with copy to parallel with copy SpeedUp', fontsize=19,
                               fontweight="bold", color="#9b86da")

            # annotate
            dfs_ax.bar_label(dfs_ax.containers[0], label_type='edge')

            # pad the spacing between the number and the edge of the figure
            dfs_ax.margins(y=1)
            dfs_ax.grid()

            dfsc_ax = dfsc.plot(kind='bar', figsize=(12, 8), legend=False, colormap='Paired',
                                ax=ax[0, 1], edgecolor="#9b86da", rot=0)

            ax[0, 1].set_xlabel('Size', fontsize=13,
                                fontweight="bold", labelpad=5, color="#9b86da")

            ax[0, 1].set_ylabel('SpeedUp[x]', fontsize=13,
                                fontweight="bold", labelpad=5, color="#9b86da")

            ax[0, 1].set_title('Sequential with copy to parallel SpeedUp', fontsize=19,
                               fontweight="bold", color="#9b86da")

            # annotate
            dfsc_ax.bar_label(dfsc_ax.containers[0], label_type='edge')

            # pad the spacing between the number and the edge of the figure
            dfsc_ax.margins(y=1)
            dfsc_ax.grid()

            dfpc_ax = dfpc.plot(kind='bar', figsize=(12, 8), legend=False, colormap='Paired',
                                ax=ax[1, 0], edgecolor="#9b86da", rot=0)

            ax[1, 0].set_xlabel('Size', fontsize=13,
                                fontweight="bold", labelpad=5, color="#9b86da")

            ax[1, 0].set_ylabel('SpeedUp[x]', fontsize=13,
                                fontweight="bold", labelpad=5, color="#9b86da")

            ax[1, 0].set_title('Sequential to parallel with copy SpeedUp', fontsize=19,
                               fontweight="bold", color="#9b86da")

            # annotate
            dfpc_ax.bar_label(dfpc_ax.containers[0], label_type='edge')

            # pad the spacing between the number and the edge of the figure
            dfpc_ax.margins(y=1)
            dfpc_ax.grid()

            dfp_ax = dfp.plot(kind='bar', figsize=(12, 8), legend=False, colormap='Paired',
                              ax=ax[1, 1], edgecolor="#9b86da", rot=0)

            ax[1, 1].set_xlabel('Size', fontsize=13,
                                fontweight="bold", labelpad=5, color="#9b86da")

            ax[1, 1].set_ylabel('SpeedUp[x]', fontsize=13,
                                fontweight="bold", labelpad=5, color="#9b86da")

            ax[1, 1].set_title('Sequential to parallel SpeedUp', fontsize=19,
                               fontweight="bold", color="#9b86da")

            # annotate
            dfp_ax.bar_label(dfp_ax.containers[0], label_type='edge')

            # pad the spacing between the number and the edge of the figure
            dfp_ax.margins(y=1)

            plot_backend = matplotlib.get_backend()
            mng = plt.get_current_fig_manager()
            if plot_backend == 'TkAgg':
                mng.resize(*mng.window.maxsize())
            elif plot_backend == 'wxAgg':
                mng.frame.Maximize(True)
            elif plot_backend == 'Qt4Agg':
                mng.window.showMaximized()

            dfp_ax.grid()

            fig.set_size_inches((22, 11), forward=False)

            plt.suptitle(
                "SPEEDUP BENCHMARK → " + f_name.upper(),
                fontsize='20',
                fontweight='bold',
                style='italic',
                family='monospace')

            plt.subplots_adjust(hspace=0.5, wspace=0.5)

            plt.savefig(d_path + f_name + '.png', dpi=500, pad_inches=0, bbox_inches='tight')

            plt.show()

            dict_seq_c_to_par_c.clear()
            dict_seq_to_par.clear()
            dict_seq_c_to_par.clear()
            dict_seq_to_par_c.clear()

    print('→ Speed up bar chart calculated successfully!\n→ File path: ' + d_path)


if __name__ == '__main__':
    #speedup_bar_chart('../build/benchmark/results/32-512_formats')
    print('Speed_up_bar chart')