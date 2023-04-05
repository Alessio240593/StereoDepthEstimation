import os
from typing import List
import pandas as pd
import matplotlib.pyplot as plt
import statistics

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

df = pd.DataFrame(columns=['size', 'time']).set_index('size')

seq_c_to_par_c = list()
seq_c_to_par = list()
seq_to_par_c = list()
seq_to_par = list()

dict_seq_c_to_par_c = dict()
dict_seq_c_to_par = dict()
dict_seq_to_par_c = dict()
dict_seq_to_par = dict()

for el in l:
    for filename in os.listdir(directory):
        f = os.path.join(directory, filename)
        if os.path.isfile(f) and "matrix: " + str(el) + "x" + str(el) in f:
            df = pd.read_csv(f)
            seq_c_to_par_c.append(float(str(df.iloc[[6]]['mean']).split('x')[0].split(' ')[4]))
            seq_c_to_par.append(float(str(df.iloc[[7]]['mean']).split('x')[0].split(' ')[4]))
            seq_to_par_c.append(float(str(df.iloc[[8]]['mean']).split('x')[0].split(' ')[4]))
            seq_to_par.append(float(str(df.iloc[[9]]['mean']).split('x')[0].split(' ')[4]))

    dict_seq_c_to_par_c[el] = statistics.mean(seq_c_to_par_c)
    dict_seq_c_to_par[el] = statistics.mean(seq_c_to_par)
    dict_seq_to_par_c[el] = statistics.mean(seq_to_par_c)
    dict_seq_to_par[el] = statistics.mean(seq_to_par)

dfs = pd.DataFrame(dict_seq_c_to_par_c.items(), columns=['size', 'time'])
dfs.set_index("size", inplace=True)
dfs.name = 'seq_c_to_par_c'

dfsc = pd.DataFrame(dict_seq_c_to_par.items(), columns=['size', 'time'])
dfsc.set_index("size", inplace=True)
dfsc.name = 'seq_c_to_par'

dfpc = pd.DataFrame(dict_seq_to_par_c.items(), columns=['size', 'time'])
dfpc.set_index("size", inplace=True)
dfpc.name = 'seq_to_par_c'

dfp = pd.DataFrame(dict_seq_to_par.items(), columns=['size', 'time'])
dfp.set_index("size", inplace=True)
dfp.name = 'seq_to_par'

fig, ax = plt.subplots(nrows=2, ncols=2)

fig.tight_layout()
ax1 = fig.add_subplot(2, 2, 1)
ax2 = fig.add_subplot(2, 2, 2)
ax3 = fig.add_subplot(2, 2, 3)
ax4 = fig.add_subplot(2, 2, 4)

dfs_ax = dfs.plot(kind='bar', figsize=(12, 8), title="Sequential with copy to parallel with copy SpeedUp",
              xlabel='Size', ylabel='SpeedUp[x]', legend=False, colormap='Paired', ax=ax1)

# annotate
dfs_ax.bar_label(dfs_ax.containers[0], label_type='edge')

# pad the spacing between the number and the edge of the figure
dfs_ax.margins(y=1)
dfs_ax.grid()

dfsc_ax = dfsc.plot(kind='bar', figsize=(12, 8), title="Sequential with copy to parallel SpeedUp",
              xlabel='Size', ylabel='SpeedUp[x]', legend=False, colormap='Paired', ax=ax2)

# annotate
dfsc_ax.bar_label(dfsc_ax.containers[0], label_type='edge')

# pad the spacing between the number and the edge of the figure
dfsc_ax.margins(y=1)
dfsc_ax.grid()

dfpc_ax = dfpc.plot(kind='bar', figsize=(12, 8), title="Sequential to parallel with copy SpeedUp",
              xlabel='Size', ylabel='SpeedUp[x]', legend=False, colormap='Paired', ax=ax3)

# annotate
dfpc_ax.bar_label(dfpc_ax.containers[0], label_type='edge')

# pad the spacing between the number and the edge of the figure
dfpc_ax.margins(y=1)
dfpc_ax.grid()

dfp_ax = dfp.plot(kind='bar', figsize=(12, 8), title="Sequential to parallel SpeedUp",
              xlabel='Size', ylabel='SpeedUp[x]', legend=False, colormap='Paired', ax=ax4)

# annotate
dfp_ax.bar_label(dfp_ax.containers[0], label_type='edge')

# pad the spacing between the number and the edge of the figure
dfp_ax.margins(y=1)
dfp_ax.grid()

#manager = plt.get_current_fig_manager()
#manager.full_screen_toggle()

plt.subplots_adjust(hspace=0.5, wspace=0.5)
plt.show()
