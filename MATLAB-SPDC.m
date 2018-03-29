clear all
close all
load 'data.dat';

% Simulation: 500 ns, 2^17 points, pump frequency 9 GHz
t = linspace(0, 500e-9, 2^17);
Fc = 4.5e9;
Fs = 1/(t(2)-t(1));

DATA = fft(data);
freqs = linspace(0, Fs, 2^17);

figure;
hold on;
plot(freqs, 20*log10(abs(DATA)));

DATA(1:10) = 0;
DATA(3500:end) = 0;
plot(freqs, 20*log10(abs(DATA)));
data = ifft(DATA, 'symmetric');

[X, Y] = demod(data, Fc, Fs, 'qam'); % demodulate to recover quadratures
n = 127;
% acquire quadrature histogram
N = histcounts2(500e6*X, 500e6*Y, [-n:n], [-n:n]);

figure;
% surface(N, 'edgecolor', 'none');
imagesc(N);
axis square;
truesize;

1e15*eig(cov([X Y])) % see how the quadratures compare