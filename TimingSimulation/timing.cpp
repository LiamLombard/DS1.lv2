#include <iostream>
#include <cmath>
#include <fstream>
#include <limits>
#include <array>
#include <vector>
#include <numeric>
#include <chrono>
#include <memory>

#include "LUT.h"
#include "DS1.h"
#include "CSVParse.h"

constexpr double PI = std::atan(1)*4;

int main()
{
  const double time_until = 1;
  const double t_step = (double) 1/44100;
  const double f = 1000;

  double output = 0;

  std::vector<std::array<long, 11>> genTimes;
  std::vector<std::array<long, 11>> simTimes;

  for(size_t runs = 0; runs < 20; ++runs)
  {
    std::array<long, 11> tempSimTimes = {0,0,0,0,0,0,0,0,0,0};
    std::array<long, 11> tempGemTimes = {0,0,0,0,0,0,0,0,0,0};

    size_t i = 0;

    while(i <= 10)
    {
      const auto filename = (i == 0) ? "res/nLUT.csv" : "res/nLUT"+ std::to_string(i) + ".csv";
      std::cout <<filename << '\n';
      const auto init = std::chrono::system_clock::now();
      std::unique_ptr<DS1> circuit = std::make_unique<DS1>(t_step, filename);

      tempGemTimes[i] = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now()-init)).count();

      const auto sim = std::chrono::system_clock::now();

      for (int t = 0; t<=(time_until/t_step); ++t)
      {
        double Vin = -3.3*std::sin(2*PI*f*t*t_step);
        output = circuit->CalcVLUT(Vin);
      }

      tempSimTimes[i] = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now()-sim)).count();

      circuit.release();
      ++i;
    }

    genTimes.push_back(tempGemTimes);
    simTimes.push_back(tempSimTimes);
  }

  std::ofstream myfile;
  myfile.open("Analysis/gentiming.csv");

  for(auto& col : genTimes)
  {
    for(auto& row : col)
    {
      myfile << row <<',';
    }
    myfile << '\n';
  }

  myfile.close();
  myfile.open("Analysis/simtiming.csv");

  for(auto& col : simTimes)
  {
    for(auto& row : col)
    {
      myfile << row <<',';
    }
    myfile << '\n';
  }
  myfile.close();

  return (int)output;
}