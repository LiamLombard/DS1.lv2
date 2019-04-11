% Boss DS1 script with LUT
close all
clear all

% Simulation parameters
% works much better with 0.001ms than 0.01ms timestep...
t_step = 1/44100;
f = 1000;
time_span = 0:t_step:5e-3
% Vin = 3.3*csvread('input.txt');
Vin = -3.3*sin(2*pi*f*(time_span));
V = zeros(8, length(Vin));

% Filling Circuit struct with useful info
circuit.t_step = t_step;
circuit.W = 0.5;

% Resistances
circuit.Rin = 0;
circuit.R1 = 2200;
circuit.R2 = 2200;
circuit.R3 = 6800;
circuit.R4_1 = (circuit.W)*20000;
circuit.R4_2 = (1-circuit.W)*20000;
circuit.R5 = 6800;
circuit.R6 = 100000;

% Capacitors
circuit.C1 = 0.47E-6;
circuit.C2 = 0.01E-6;
circuit.C3 = 0.022E-6;
circuit.C4 = 0.1E-6;

% Conductances
circuit.G1 = 1/circuit.R1;
circuit.G2 = 1/circuit.R2;
circuit.G3 = 1/circuit.R3;
circuit.G4_1 = 1/circuit.R4_1;
circuit.G4_2 = 1/circuit.R4_2;
circuit.G5 = 1/circuit.R5;
circuit.G6 = 1/circuit.R6;

circuit.Gc1 = Gc(circuit.C1, circuit.t_step);
circuit.Gc2 = Gc(circuit.C2, circuit.t_step);
circuit.Gc3 = Gc(circuit.C3, circuit.t_step);
circuit.Gc4 = Gc(circuit.C4, circuit.t_step);

% Linear conductance matrix (diodes counted later)
circuit.G = CalcLinG(circuit);

no=1

% Lookup table matrix
circuit.LUT = csvread("../res/nLUT"+no+".csv");

% Simulation loop
for n = 1:length(Vin)-1
  Il = CalcLinCurrents(V(:, n), Vin(n+1), circuit);
  V(:, n+1) = IterateV(V(:, n), Il, circuit);
end

dlmwrite('../res/LUT' + no + 'output.csv', [time_span; V(7, :)]', 'delimiter', ',', 'precision', 16);

function v_next = IterateV(v_last, Il, cir)
  [gd, ieq] = lookup(v_last(3), cir.LUT);
  I = StampDiodeCurrents(Il, v_last, ieq);
  G = StampGDiodes(v_last, cir, gd);
  v_next = G\I;
end

% Equations here generated from a different script
% function v_next = GaussianElimination(I, gd)

%   v_next =
% end

function g = CalcLinG(cir)
  g1 = cir.G1;
  g2 = cir.G2;
  g3 = cir.G3;
  g4_1 = cir.G4_1;
  g4_2 = cir.G4_2;
  g5 = cir.G5;
  g6 = cir.G6;
  gc1 = cir.Gc1;
  gc2 = cir.Gc2;
  gc3 = cir.Gc3;
  gc4 = cir.Gc4;
  Rin = cir.Rin;

  g = [g1 -g1 0 0 0 0 0 1;
      -g1 g1+gc1 -gc1 0 0 0 0 0;
      0 -gc1 gc1+gc2+gc3+g3 -gc3 -g3 0 0 0;
      0 0 -gc3 g2+gc3 0 -g2 0 0;
      0 0 -g3 0 g3+gc4+g4_1 0 -g4_1 0;
      0 0 0 -g2 0 g2+g4_2+g5 -g4_2 0;
      0 0 0 0 -g4_1 -g4_2 g4_1+g4_2+g6 0;
      -1 0 0 0 0 0 0 -Rin];
end

function g = StampGDiodes(v_last, cir, gd)
  g = cir.G;
  g(3, 3) = g(3, 3) + gd;
end

function i = CalcLinCurrents(V, Vin, cir)
  i = [0;
      -Ic(V(2)-V(3), cir.C1, cir.t_step);
       Ic(V(2)-V(3), cir.C1, cir.t_step) - Ic(V(3), cir.C2, cir.t_step) - Ic(V(3)-V(4), cir.C3, cir.t_step);
       Ic(V(3)-V(4), cir.C3, cir.t_step);
       -Ic(V(5), cir.C4, cir.t_step);
       0;
       0;
      -Vin];
end

function ii = StampDiodeCurrents(I, v_last, id)
  ii = I;
  ii(3) = ii(3) + id;
end

function g = Gd(v_m)
  g = (1.5e-15/26e-3)*exp(v_m/26e-3);
end

function i = Ieq(v_m)
  i = Id(v_m) - Gd(v_m)*v_m;
end

function i = Id(v_m)
  i = 1.5e-15*(exp(v_m/26e-3) - 1);
end

function i = Ic(v_last, C, t_step)
  i = -v_last*Gc(C, t_step);
end

function g = Gc(C, t_step)
  g = C/t_step;
end

function [gd ieq] = lookup(v_m, LUT)
  [~, index] = min(abs(LUT(:,1)-v_m));
  comparison = zeros(1, 4);
  if LUT(index,1)-v_m > 0 && index > 1
    comparison = LUT(index-1,:);
    interped = interpolateVec(v_m, comparison, LUT(index,:));
  elseif LUT(index,1)-v_m < 0 && index < length(LUT)
    comparison = LUT(index+1,:);
    interped = interpolateVec(v_m, LUT(index,:), comparison);
  else
    if index == 1 || index == length(LUT)
      fprintf(2, 'Looking up value outside of lookup tables range: %f -> index: %d\n', v_m, index);
    end
    gd = LUT(index,2);
    ieq = LUT(index,3);
    return
  end
  gd = interped(2);
  ieq = interped(3);
end

function out = interpolateVec(v_m, comp1, comp2)
  out = zeros(size(comp1));
  out(1) = v_m;
  x2 = comp1(1);
  x1 = comp2(1);
  for i = 2:length(comp1)
    y2 = comp1(i);
    y1 = comp2(i);

    out(i) = y2 - ((x2-v_m)*(y2-y1)/(x2-x1));
  end
end