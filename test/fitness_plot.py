#!/usr/bin/python3
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

avg_n = 100

if len(sys.argv) < 3:
    print("error provide csv name and header row #")

data_file = sys.argv[1]
base_name = sys.argv[1].split(".")[0]
print("working on {}".format(sys.argv[1]))

def moving_average(x, w):
    return np.convolve(x, np.ones(w), 'valid') / w

data = pd.read_csv(data_file, header=int(sys.argv[2]))

fig, ax = plt.subplots()

ax.plot(data['generation'], data['avg_fitness'])
ax.plot(data['generation'][avg_n//2:(data.shape[0])-avg_n//2 +1], moving_average(data['avg_fitness'], avg_n))
plt.title("Average eater fitness over generations")
ymax = max(data['avg_fitness'])
xpos = data['avg_fitness'].idxmax()
xmax = data['generation'][xpos]
ax.scatter(xmax, ymax, c='r', marker='x')
fig.savefig(base_name + "_avg_fitness.png")
ax.clear()

ax.plot(data['generation'], data['avg_fitness_apex'])
ax.plot(data['generation'][avg_n//2:(data.shape[0])-avg_n//2 +1], moving_average(data['avg_fitness_apex'], avg_n))
plt.title("Average apex fitness over generations")
ymax = max(data['avg_fitness_apex'])
xpos = data['avg_fitness_apex'].idxmax()
xmax = data['generation'][xpos]
ax.scatter(xmax, ymax, c='r', marker='x')
fig.savefig(base_name + "_avg_fitness_apex.png")
ax.clear()

ax.plot(data['generation'], data['max_fitness'])
ax.plot(data['generation'][avg_n//2:(data.shape[0])-avg_n//2 +1], moving_average(data['max_fitness'], avg_n))
plt.title("Maximum eater fitness over generations")
ymax = max(data['max_fitness'])
xpos = data['max_fitness'].idxmax()
xmax = data['generation'][xpos]
ax.scatter(xmax, ymax, c='r', marker='x')
fig.savefig(base_name + "_fitness.png")
ax.clear()

ax.plot(data['generation'], data['max_fitness_apex'])
ax.plot(data['generation'][avg_n//2:(data.shape[0])-avg_n//2 +1], moving_average(data['max_fitness_apex'], avg_n))
plt.title("Maximum apex fitness over generations")
ymax = max(data['max_fitness_apex'])
xpos = data['max_fitness_apex'].idxmax()
xmax = data['generation'][xpos]
ax.scatter(xmax, ymax, c='r', marker='x')
fig.savefig(base_name + "_fitness_apex.png")
