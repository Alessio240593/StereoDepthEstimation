from typing import List

import matplotlib
import numpy as np
from matplotlib import pyplot as plt


mu, sigma = 0.5, 0.5  # mean and standard deviation

s = np.random.lognormal(mu, sigma, 4)

plot_backend = matplotlib.get_backend()
mng = plt.get_current_fig_manager()
if plot_backend == 'TkAgg':
    mng.resize(*mng.window.maxsize())
elif plot_backend == 'wxAgg':
    mng.frame.Maximize(True)
elif plot_backend == 'Qt4Agg':
    mng.window.showMaximized()


# Creating axes instance
bp = plt.boxplot(s, patch_artist=True,
                 notch='True', vert=0, showmeans=True, meanprops={'markerfacecolor': 'red', 'markeredgecolor': 'black', 'markersize': '12'}, showfliers=True)

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

plt.xlabel("Time[ms]", fontweight='bold', style='italic', fontsize=13)


plt.grid()

plt.show()


# Creating axes instance
bp = plt.boxplot(s, patch_artist=True, vert=0, showmeans=True, meanprops={'markerfacecolor': "red", 'markeredgecolor': 'black'})

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

plt.xlabel("Time[ms]", fontweight='bold', style='italic', fontsize=13)


plt.grid()

plt.show()