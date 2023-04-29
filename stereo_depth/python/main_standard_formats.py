from speed_up import speed_up
from speed_up_bar_chart import speedup_bar_chart
from time_series import plot_time_series
from time_series_with_error import plot_time_series_with_error
from time_series_boxplot_standard_formats import time_series_boxplot

speed_up('../build/benchmark/results/standard_formats')
plot_time_series('../build/benchmark/results/standard_formats')
plot_time_series_with_error('../build/benchmark/results/standard_formats')
speedup_bar_chart('../build/benchmark/results/standard_formats')
time_series_boxplot('../build/benchmark/results/standard_formats')
