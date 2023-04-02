#!/usr/bin/bash

{ time ./gen > gen.csv; } 2> time_err
cat gen.csv >> time_err
cp time_err gen.csv
rm time_err

