import csv
import pandas as pd
import numpy as np
from scipy.interpolate import make_interp_spline
import matplotlib.pyplot as plt

def plotRTDSmooth2(col, col_name, N):
    pass

def plotRTDSmooth(col, col_name, N):
    x = np.sort(np.array([c for c in col]))
    y = np.sort(np.array([i / N for i in range(N)]))

    print(x)
    print(y)

    X_Y_Spline = make_interp_spline(x, y)
    XX = np.linspace(x.min(), x.max(), 500)
    YY = X_Y_Spline(XX)

    # plotting a line graph
    plt.plot(XX, YY, label=col_name)

def plotRTDp(col, col_name, N):

    # creating dataframe
    df = pd.DataFrame({
        'X': col,
        'Y': [i/N for i in range(N)]
    })

    # plotting a line graph
    plt.plot(df["X"], df["Y"], label=col_name)

def plotAllRTD(all_timings, cols, N, algo):
    for c in cols:
        all_timings.sort_values(by=[c], inplace=True)
        plotRTDp(all_timings[c], c, N)
    plt.legend()
    plt.title("Qualified Run-time distributions - " + algo)
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

    N = 5

    all_timings_GA = getTimingsP(filename_RTD1)
    plotAllRTD(all_timings_GA, ["BK", "GA01", "GA05", "GA1", "GA2"], N, "GA")

    all_timings_TS = getTimingsP(filename_RTD2)
    plotAllRTD(all_timings_TS, ["BK", "TS01", "TS05", "TS1", "TS2"], N, "TS")



if __name__ == "__main__":
    main()