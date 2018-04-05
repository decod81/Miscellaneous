%% numerically controlled oscillator
clc
clear all
close all

N = 2^14; % table size (1/4 of the size if symmetries were utilized)
t = linspace(0, 1, N);
SINTABLE = round((2^15-1)*cos(2*pi*t)); % 16-bit signed integers

% f = 8191;
f = 100;    % frequency
p = 1000.5; % phase

for n = 1:2^16
    y(n) = SINTABLE(1+mod(round(f*n), N-1));
    z(n) = SINTABLE(1+mod(round(f*n + p), N-1));
end

figure; hold on; plot(y); plot(z);
figure; plot(10*log10(abs(fft(y)).^2));
figure; plot(diff(SINTABLE));

Y = fft(z);
[YM, I] = max(abs(Y));
angle(Y(I(1)))
%%
clc
clear all
close all

N = 2^14;
t = linspace(0, 1/4, N);
SINTABLE = cos(2*pi*t);

figure;
hold on;
plot(t, SINTABLE);