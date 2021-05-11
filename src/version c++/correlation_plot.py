import csv
import matplotlib.pyplot as plt

def correlationPlot(all_avg_rpds, job_nb):
    x = all_avg_rpds[job_nb]["GA"]
    y = all_avg_rpds[job_nb]["TS"]
    fig = plt.figure(figsize=(700 / 72, 500 / 72), dpi=72)
    ax = plt.subplot()
    ax.set_title(f"Correlation plot between average relative percentage deviations of GA and TS - {job_nb} jobs")
    ax.scatter(x, y, s=15, marker="D")
    ax.set_xlabel("SLS 1 : Genetic algorithm : average RPDs")
    ax.set_ylabel("SLS 2 : Tabu search : average RPDs")
    xy = x + y
    min_lim = min(xy) - 0.05
    max_lim = max(xy) + 0.05
    ax.set_xlim([min_lim, max_lim])
    ax.set_ylim([min_lim, max_lim])
    plt.tight_layout()
    plt.show()

def getAvgRPDs():
    all_avg_rpds = {50: {"GA": [], "TS": []}, 100: {"GA": [], "TS": []}}
    with open('results/SLS_avgrunsRPDs_results.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            if row[0] == "instance":
                continue
            job_nb = 50
            if row[0].split("_")[0] == "100":
                job_nb = 100
            all_avg_rpds[job_nb]["GA"].append(float(row[1]))
            all_avg_rpds[job_nb]["TS"].append(float(row[2]))
            line_count += 1

    return all_avg_rpds

def main():
    all_avg_rpds = getAvgRPDs()
    print(all_avg_rpds)
    correlationPlot(all_avg_rpds, 50)
    correlationPlot(all_avg_rpds, 100)

if __name__ == "__main__":
    main()