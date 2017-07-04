clc; clear all; close all;
energy_range = [0.1 5];  % MeV

%% EStar
estar_data = dlmread('HeIG4H10_estar.txt', '', 17);
rhoHe = 0.1786; % kg/m3
rhoiH4C10 = 2.51; % kg/m3
rho = (0.8*rhoHe + 0.2*rhoiH4C10)*1e-3; % g/cm3

estar_E = estar_data(:,1);  % MeV
energy_inds = estar_E >= energy_range(1) & estar_E <= energy_range(2);
estar_E_selected = estar_E(energy_inds);
estar_losses = estar_data(energy_inds,2);  % MeV*cm2/g
estar_dEdx = estar_losses*1e3; % keV*cm^2/g

figure(1)
plot(estar_E_selected, estar_dEdx, 'o-');
% title('EStar e^{-} energy losses in He:iH_{4}C_{10} 80:20')
xlabel('Kinetic energy, MeV')
ylabel('dE/dx, keV*cm{^2}')
grid on
set(gcf, 'color', 'w');

%% HEED
heed_data = dlmread('HEED.txt', '', 1);
heed_E = heed_data(:,1)*1e-6 - 0.511;  % MeV
heed_dEdx = heed_data(:,2)*1e-3 /3*10 /rho; % keV*cm^2/g

hold on
    plot(heed_E, heed_dEdx, 'ro-');
hold off


%% Legend
legend('EStar', 'HEED')



%% Checkout 
figure(2)
plot(heed_E, estar_dEdx./heed_dEdx, 'o')
xlabel('Kinetic energy, MeV')
ylabel('$( \frac{dE}{dx} )_{EStar} / ( \frac{dE}{dx} )_{HEED} $','interpreter','latex','fontsize',16)
grid on
set(gcf, 'color', 'w');


