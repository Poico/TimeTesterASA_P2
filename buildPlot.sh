#!/bin/bash

g++ -std=c++11 -O3 -Wall -o gen_tuganet gen_tuganet.cpp -lm
echo "Please enter the name of the executable of your Project:"
read executable

mkdir -p tests

for i in $(seq 1000 500 10000);
do echo $i;
    ./gen_tuganet $i $(((i*i)/2)) 100 1 $i > tests/testfile$i
    
done

for file in tests/testfile*;
do
    echo "-----------------------";
    echo $file >> time_tests.txt;
    { time ./$executable < $file; } 2>> time_tests.txt;
    echo "-----------------------";
    
done

# Extract data and write to a new file
awk '/^tests\/testfile/{gsub("tests/testfile", ""); num=$0} /^real/{gsub("real\t", ""); split($0, a, "m"); time=a[1]*60 + a[2]; print num, time}' time_tests.txt > plot_data.txt

sort -n plot_data.txt > plot_data_sorted.txt
awk '{print (1000+ (NR-1)*500)+((1000 + (NR-1)*500)^2)/2, $2}' plot_data_sorted.txt > plot_data_sorted2.txt
awk '{print ((1000 + (NR-1)*500)^2)/2, $2}' plot_data_sorted.txt > plot_data_sorted3.txt
gnuplot -p -e "

set multiplot layout 3,1 title 'Multiplot';

set xlabel 'Number of Vertices';
set ylabel 'Execution Time (s)';
plot 'plot_data_sorted.txt' using 1:2 smooth csplines with lines title 'V';

set xlabel 'Vertices + Edges';
set ylabel 'Execution Time (s)';
plot 'plot_data_sorted2.txt' using 1:2 with lines title 'V+E';

set xlabel 'Edges in Graph';
set ylabel 'Execution Time (s)';
plot 'plot_data_sorted3.txt' using 1:2 with lines title 'E';

unset multiplot;"

rm time_tests.txt
rm -r tests
rm plot_data.txt
rm plot_data_sorted.txt
rm plot_data_sorted2.txt
rm plot_data_sorted3.txt
rm gen_tuganet
