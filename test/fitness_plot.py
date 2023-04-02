#!/usr/bin/python3
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

avg_n = 250

if len(sys.argv) < 2:
    print("error provide csv name")

data_file = sys.argv[1]
base_name = sys.argv[1].split(".")[0]
print("working on {}".format(sys.argv[1]))

def moving_average(x, w):
    return np.convolve(x, np.ones(w), 'valid') / w

data = pd.read_csv(data_file, header=3)

fig, ax = plt.subplots()
ax.plot(data['generation'], data['max_fitness'])
ax.plot(data['generation'][avg_n//2:(data.shape[0])-avg_n//2 +1], moving_average(data['max_fitness'], avg_n))

fig.savefig(base_name + "_fitness.png")
ax.clear()
ax.plot(data['generation'], data['avg_fitness'])
ax.plot(data['generation'][avg_n//2:(data.shape[0])-avg_n//2 +1], moving_average(data['avg_fitness'], avg_n))
fig.savefig(base_name + "_avg_fitness.png")
