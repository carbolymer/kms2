clear;

kmax = 100;

energy1 = load('-ascii','s1e-4/E.dat')(2:kmax,2);
energy1 = energy1./energy1(2);
energy2 = load('-ascii','s1.01e-4/E.dat')(2:kmax,2);
energy2 = energy2./energy2(2);
energy3 = load('-ascii','s1.0005e-4/E.dat')(2:kmax,2);
energy3 = energy3./energy3(2);


maximumEnergy = [ 8.585 10.019 12.151 15.066 18.237 19.774 18.624 15.844 13.105 11.063 9.611];
omega = [0.9 0.92 0.94 0.96 0.98 1 1.02 1.04 1.06 1.08 1.1];


% k = 1:1:size(energy1)(1);
k = 2:1:kmax;
k = k.*10;

figure(1);
hold on;
plot(k,energy1,k,energy2,k,energy3);

hold off;
axis([0 k(end) 0.99 1.05]);
axis 'autox';
ylabel('Wzgledna wartosc energii <E>_k/<E>_{k=0}');
xlabel('Numer kroku k');
legend('\tau = 1\cdot10^{-4}', '\tau = 1.01\cdot10^{-4}', '\tau = 1.0005\cdot10^{-4}');
print('stability.png','-S640,500');
close(1);

figure(2);
plot(omega, maximumEnergy,'.0',omega,maximumEnergy,'-0');
axis([0.88 1.12 0 1]);
axis 'autoy';
ylabel('Energia <E>_{max}');
xlabel('Czestotliwosc \omega / \omega_0');
print('resonance.png','-S640,500');
close(2);
