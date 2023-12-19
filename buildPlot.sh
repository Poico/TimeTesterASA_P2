#!/bin/bash

g++ -std=c++11 -O3 -Wall -o gen_tuganet gen_tuganet.cpp -lm 
echo "Please enter the name of the executable of your Project:"
read executable

mkdir -p tests

for i in $(seq 50000 100 100000);
do echo $i;
    ./gen_tuganet $i $i 100 1 1000 > tests/testfile$i
    
done

for file in tests/testfile*;
do echo "-----------------------";
    echo $file >> time_tests.txt;
    { time ./$executable < $file; } 2>> time_tests.txt;
done

# Extract data and write to a new file
awk '/^tests\/testfile/{gsub("tests/testfile", ""); num=$0} /^real/{gsub("real\t", ""); split($0, a, "m"); time=a[1]*60 + a[2]; print num, time}' time_tests.txt > plot_data.txt

sort -n plot_data.txt > plot_data_sorted.txt

gnuplot -p -e "set xlabel 'Testfile Number'; set ylabel 'Execution Time (s)'; set yrange [0:0.3]; plot 'plot_data_sorted.txt' using 1:2 smooth csplines with lines title 'Execution Time'"

rm time_tests.txt
rm -r tests
rm plot_data.txt
rm plot_data_sorted.txt
rm gen_tuganet