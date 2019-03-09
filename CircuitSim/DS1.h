#pragma once

#include <cstdint>
#include <array>
#include <fstream>
#include "LUT.h"

constexpr std::size_t C_SIZE = 8;


class DS1
{
  private:
    const double C1 = 0.47E-6;
    const double C2 = 0.01E-6;
    const double C3 = 0.022E-6;
    const double C4 = 0.1E-6;
    const double t_step;
    std::ofstream logger;

    LUT lookup;

    std::array<double, C_SIZE> v_last = {0,0,0,0,0,0,0,0};

  public:
    DS1(double tt_step, const std::string& fileName);

    double Gc(double C);
    double Gd(double v_m);
    double Ieq(double v_m);
    double Ic(double v_across, double C);

    double GaussianEllimination(const std::array<double, C_SIZE> i, const double gd);
    double CalcV(const double vin);
    std::array<double, C_SIZE> StampDiodeCurrents(std::array<double, C_SIZE> I);
    std::array<double, C_SIZE> CalcLinCurrents(const double vin);

    double CalcVLUT(const double vin);
    std::array<double, C_SIZE> StampDiodeCurrentsLUT(std::array<double, C_SIZE> I, const double ieq);
};


// struct Circuit
// {
//   const double time_until = 5e-3;
//   const double t_step = 0.01e-3;
//   const double f = 1000;
//   const double W = 0.5;
//   const double W2 = 0.5;

//   // Resistances
//   const double Rin = 0;
//   const double R1 = 2200;
//   const double R2 = 6800;
//   const double R3 = 2200;
//   const double R4_1 = (W)*20000;
//   const double R4_2 = (1-W)*20000;
//   const double R5 = 6800;
//   const double R6 = (W2)*100000;

//   // Capacitors
//   const double C1 = 0.47E-6;
//   const double C2 = 0.01E-6;
//   const double C3 = 0.022E-6;
//   const double C4 = 0.1E-6;

//   // Conductances
//   const double G1 = 1/R1;
//   const double G2 = 1/R2;
//   const double G3 = 1/R3;
//   const double G4_1 = 1/R4_1;
//   const double G4_2 = 1/R4_2;
//   const double G5 = 1/R5;
//   const double G6 = 1/R6;

//   const double Gc1 = Gc(C1, t_step);
//   const double Gc2 = Gc(C2, t_step);
//   const double Gc3 = Gc(C3, t_step);
//   const double Gc4 = Gc(C4, t_step);
// };