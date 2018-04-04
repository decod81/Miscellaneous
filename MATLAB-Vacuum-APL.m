%% create quantum noise
clc
clear all
close all

N = 8;
t = linspace(0, N*500e-9, N*2^16);
N = sqrt(linspace(0, 2, N*2^16)).*exp(j*2*pi*rand(1, N*2^16));
figure; plot(abs(N).^2);
noise = ifft(N, 'symmetric');
v = [t' noise'];
save('vnoise.dat', 'v', '-ascii');
%% analyze aplac simulation
clc
clear all
close all

N = 8;
load 'data.dat'

f = fft(data(:, 1));
g = fft(data(:, 2));
p = dot(data(:, 1), data(:, 1))/dot(data(:, 2), data(:, 2))

figure;
plot(abs(xcorr(f, f, 'coeff')));

a = resample(abs(f).^2, 1, 512);
b = resample(abs(g).^2, 1, 512);
c = resample(linspace(0, 2^17, N*2^16), 1, 512);

figure;
plot(c, 0.5*a./b);
xlim([0 10000]);
ylim([0.5 0.8]);