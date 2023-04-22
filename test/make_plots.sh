#!/usr/bin/bash

for i in {0..3}
do
    python3 fitness_plot.py test$i\_$1.csv test$i\_$1
done