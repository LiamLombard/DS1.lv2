#include <iostream>
#include <cmath>
#include <fstream>
#include <limits>
#include <array>

#include "LUT.h"
#include "DS1.h"
#include "CSVParse.h"

constexpr double PI = std::atan(1)*4;

int main()
{
  const double time_until = 5e-3;
  const double t_step = (double) 1/44100;
  const double f = 1000;

  DS1 circuit(t_step, "res/nLUT9.csv");

  std::ofstream myfile;
  myfile.open ("Analysis/data.csv");



   for (int t = 0; t<=(time_until/t_step); ++t)
   {
      double Vin = -3.3*std::sin(2*PI*f*t*t_step);

      auto v = circuit.CalcVLUT(Vin);

      myfile << t*t_step << ',' << v << '\n';
   }

   myfile.close();
   return 0;
}