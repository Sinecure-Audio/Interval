#!/bin/bash 
cd ./tests && mkdir testbuild && cd testbuild && cmake -G Ninja ../ && ninja && touch ../../../testResults.xml && ./IntervalTests -o ../../../testResults.xml -r junit || true