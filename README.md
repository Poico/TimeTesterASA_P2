# TimeTesterASA_P2

## Overview
TimeTesterASA_P2 is a performance analysis tool designed to measure and visualize the execution time of the second project of ASA (Algorithm and Data Structures). It leverages the power of gnuplot to generate insightful plots from the collected data.

## Getting Started

### Prerequisites
- C++ compiler with support for C++11
- gnuplot

### Installation
1. Clone the repository to your local machine using `git clone`.
2. Ensure you have gnuplot installed. If not, you can download it from [here](http://www.gnuplot.info/download.html).

## Usage
Follow these steps to compile and run the project:

#### 1. Compile the C++ file using the following command:
  g++ -std=c++11 -O3 -Wall -o fileName fileName.cpp -lm

#### 2. Make the script executable:
  chmod +xr buildPlot.sh

#### 3. Run the script:
  ./buildPlot.sh

## License
This project is licensed under the terms of the MIT License. See the [LICENSE](LICENSE) file for details.