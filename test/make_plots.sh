#!/usr/bin/bash

for i in {0..19}
do
    python3 fitness_plot.py "test$i.csv" "test$i"
done