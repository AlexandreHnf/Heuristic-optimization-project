import csv
import pandas as pd
import matplotlib.pyplot as plt

def plotRTD(all_timings):
    x = all_avg_rpds[job_nb]["GA"]
    y = all_avg_rpds[job_nb]["TS"]
    fig = plt.figure(figsize=(700 / 72, 500 / 72), dpi=72)
    ax = plt.subplot()
    ax.set_title(f"Correlation plot between average relative percentage deviations of GA and TS - {job_nb} jobs")
    ax.scatter(x, y, s=15, marker="D")
    ax.set_xlabel("SLS 1 : Genetic algorithm : average RPDs")
    ax.set_ylabel("SLS 2 : Tabu search : average RPDs")
    plt.tight_layout()
    plt.show()

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

    all_timings_GA = getTimingsP(filename_RTD1)
    plotAllRTD(all_timings_GA, ["BK", "GA01", "GA05", "GA1", "GA2"], 15, "GA")

    all_timings_TS = getTimingsP(filename_RTD2)
    plotAllRTD(all_timings_TS, ["BK", "TS01", "TS05", "TS1", "TS2"], 15, "TS")



if __name__ == "__main__":
    main()