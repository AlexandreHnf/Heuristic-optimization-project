import csv
import pandas as pd
import numpy as np
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt

def plotRTDSmooth(col, col_name, N):
    df = pd.DataFrame({
        'X': col,
        'Y': [i / N for i in range(N)]
    })
    f = np.poly1d(np.polyfit(df["X"], df["Y"], deg=3))
    x = np.linspace(np.amin(df["X"]), np.amax(df["X"]), 500)
    y = f(x)
    plt.plot(x, y, label=col_name)

def plotRTDp(col, col_name, N):

    # creating dataframe
    df = pd.DataFrame({
        'X': col,
        'Y': [i/N for i in range(N)]
    })

    # plotting a line graph
    plt.plot(df["X"], df["Y"], label=col_name)

def plotAllRTD(all_timings, cols, N, algo, instance_name, smooth=False):
    for c in cols:
        all_timings.sort_values(by=[c], inplace=True)
        if smooth:
            plotRTDSmooth(all_timings[c], c, N)
        else:
            plotRTDp(all_timings[c], c, N)
    plt.legend()
    plt.title(f"Qualified Run-time distributions - {algo} - {N} iterations - instance {instance_name}")
    plt.ylabel('P(solve)')
    plt.xlabel('Run-time [CPU sec]')
    plt.show()

def getTimingsP(filename):
    all_timings = pd.read_csv(filename)
    # print(all_timings)

    return all_timings

def main():
    filename_RTD1 = "results/SLS_RTD_1.csv"
    filename_RTD2 = "results/SLS_RTD_2.csv"

    N = 25

    all_timings_inst1 = getTimingsP(filename_RTD1)

    plotAllRTD(all_timings_inst1, ["BK", "GA01", "GA05", "GA1", "GA2"], N, "GA", "50_20_01")
    plotAllRTD(all_timings_inst1, ["BK", "GA01", "GA05", "GA1", "GA2"], N, "GA", "50_20_01", True) # smooth

    plotAllRTD(all_timings_inst1, ["BK", "TS01", "TS05", "TS1", "TS2"], N, "TS", "50_20_01")
    plotAllRTD(all_timings_inst1, ["BK", "TS01", "TS05", "TS1", "TS2"], N, "TS", "50_20_01", True) # smooth

    all_timings_inst2 = getTimingsP(filename_RTD2)

    plotAllRTD(all_timings_inst2, ["BK", "GA01", "GA05", "GA1", "GA2"], N, "GA", "50_20_02")
    plotAllRTD(all_timings_inst2, ["BK", "GA01", "GA05", "GA1", "GA2"], N, "GA", "50_20_02", True)  # smooth

    plotAllRTD(all_timings_inst2, ["BK", "TS01", "TS05", "TS1", "TS2"], N, "TS", "50_20_02")
    plotAllRTD(all_timings_inst2, ["BK", "TS01", "TS05", "TS1", "TS2"], N, "TS", "50_20_02", True)  # smooth



if __name__ == "__main__":
    main()