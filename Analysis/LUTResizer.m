LUT = csvread("../res/nLUT.csv");

% dlmwrite('nLUT1.csv', LUT([1:2:end],:), 'delimiter', ',', 'precision', 16);
% dlmwrite('nLUT2.csv', LUT([1:3:end],:), 'delimiter', ',', 'precision', 16);
% dlmwrite('nLUT3.csv', LUT([1:4:end],:), 'delimiter', ',', 'precision', 16);
% dlmwrite('nLUT4.csv', LUT([1:5:end],:), 'delimiter', ',', 'precision', 16);
% dlmwrite('nLUT5.csv', LUT([1:6:end],:), 'delimiter', ',', 'precision', 16);
% dlmwrite('nLUT6.csv', LUT([1:10:end],:), 'delimiter', ',', 'precision', 16);
% dlmwrite('nLUT7.csv', LUT([1:20:end],:), 'delimiter', ',', 'precision', 16);
% dlmwrite('nLUT8.csv', LUT([1:50:end],:), 'delimiter', ',', 'precision', 16);
% dlmwrite('nLUT9.csv', LUT([1:100:end],:), 'delimiter', ',', 'precision', 16);
dlmwrite('../res/nLUT10.csv', LUT([1:1000:end],:), 'delimiter', ',', 'precision', 16);