import os
import pandas as pd


def calculate_speed_up(df: pd.DataFrame, f: str):
    """
    Calcola lo speed up per il file f passato come parametro

    :param df:  Data frame del file corrente
    :param f:   File corrente
    :return:    Lo speed up calcolato per il file f
    :rtype:     void
    """

    seq_c_mean = float(df.iloc[[0]]['mean'])
    seq_c_median = float(df.iloc[[0]]['median'])
    seq_mean = float(df.iloc[[1]]['mean'])
    seq_median = float(df.iloc[[1]]['median'])
    par_c_mean = float(df.iloc[[2]]['mean'])
    par_c_median = float(df.iloc[[2]]['median'])
    par_mean = float(df.iloc[[3]]['mean'])
    par_median = float(df.iloc[[3]]['median'])

    data = [{'id': '', 'mean': '', 'std': '', 'median': ''},
            {'id': 'SpeedUp', 'mean': 'Mean', 'std': '', 'median': 'Median'},
            {'id': 'seq_c → par_c', 'mean': str(round(seq_c_mean / par_c_mean, 1)) + 'x', 'std': '',
             'median': str(round(seq_c_median / par_c_median, 1)) + 'x'},
            {'id': 'seq_c → par', 'mean': str(round(seq_c_mean / par_mean, 1)) + 'x', 'std': '',
             'median': str(round(seq_c_median / par_median, 1)) + 'x'},
            {'id': 'seq → par_c', 'mean': str(round(seq_mean / par_c_mean, 1)) + 'x', 'std': '',
             'median': str(round(seq_median / par_c_median, 1)) + 'x'},
            {'id': 'seq → par', 'mean': str(round(seq_mean / par_mean, 1)) + 'x', 'std': '',
             'median': str(round(seq_median / par_median, 1)) + 'x'}]

    new_df = pd.DataFrame(data)

    dff = pd.concat([df, new_df])
    dff.reset_index(drop=True)

    dff.to_csv(f, index=False)


def speed_up(directory: str):
    """
    Calcola lo speed up per ogni file nella directory passata come parametro

    :param directory:   Directory contenente i file di interesse per il calcolo dello speed up
    :return:            Scrive lo speed up direttamente in ogni file
    :rtype:     void
    """
    # Per ogni file calcolo lo speedup
    if not os.path.exists(directory):
        raise NotADirectoryError(directory, "not exists")

    for filename in os.listdir(directory):
        f = os.path.join(directory, filename)
        if os.path.isfile(f):
            df = pd.read_csv(f)
            if 'SpeedUp' in df.values:
                continue
            else:
                calculate_speed_up(df, f)
    print('→ Speed up calculated successfully!\n→ File path: ' + directory)


if __name__ == '__main__':
    speed_up('../build/benchmark/results/32-512_formats')