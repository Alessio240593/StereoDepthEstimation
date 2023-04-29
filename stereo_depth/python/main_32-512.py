from speed_up import speed_up
from speed_up_bar_chart import speedup_bar_chart
from time_series import plot_time_series
from time_series_boxplot import time_series_boxplot
from time_series_with_error import plot_time_series_with_error

speed_up('../build/benchmark/results/32-512_formats')
plot_time_series('../build/benchmark/results/32-512_formats')
plot_time_series_with_error('../build/benchmark/results/32-512_formats')
speedup_bar_chart('../build/benchmark/results/32-512_formats')
time_series_boxplot('../build/benchmark/results/32-512_formats')
