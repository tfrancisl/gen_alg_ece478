#!/usr/bin/python3
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

avg_n = 100

if len(sys.argv) < 3:
    print("error provide csv name and output basename")

data_file = sys.argv[1]
base_name = sys.argv[2]
print("working on {}".format(sys.argv[1]))

def moving_average(x, w):
    return np.convolve(x, np.ones(w), 'valid') / w

data = pd.read_csv(data_file, header=0)

def make_plot(xdata, ydata, title="", name_ext=".png"):
    fig, ax = plt.subplots()
    
    
    line1, = ax.plot(xdata, ydata)
    line2, = ax.plot(xdata[avg_n//2:(data.shape[0])-avg_n//2 +1], moving_average(ydata, avg_n))
    ax.legend([line1, line2],["All generations", f"n={avg_n} moving average"])

    ax.set_xlabel("Generation")
    ax.set_ylabel("Fitness")
    plt.title(title)
    ymax = max(ydata)
    xpos = ydata.idxmax()
    xmax = xdata[xpos]
    ax.scatter(xmax, ymax, c='r', marker='x')
    fig.savefig(base_name + name_ext)
    ax.clear()


make_plot(data['generation'], data['avg_fitness'], "Average eater fitness over generations", "_avg_fitness.png")
make_plot(data['generation'], data['avg_fitness_apex'], "Average apex fitness over generations", "_apex_avg_fitness.png")
make_plot(data['generation'], data['max_fitness'], "Max eater fitness over generations", "_max_fitness.png")
make_plot(data['generation'], data['max_fitness_apex'], "Max eater fitness over generations", "_apex_max_fitness.png")