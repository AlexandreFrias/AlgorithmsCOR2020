#!/bin/bash

./teste 2 > result0.csv &
./teste 3 > result1.csv &
./teste 4 > result2.csv &
./kojic > result3.csv &

wait
