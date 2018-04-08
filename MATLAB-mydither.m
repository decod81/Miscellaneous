close all
clear all

A = 0; 
% A = 0.1;
B = 1;
% B = 0.15;
M = 16;
% M = 256;
X = 1200;
Y = 256;
img = zeros(Y, X);
for x = 1:Y
    img(x, :) = linspace(A, B, X);
end

figure;
im = round((M-1)*img);
image(uint8(im));
colormap(gray(M));
truesize;

pix = zeros(Y, X);
for y = 1:Y
    for x = 1:X
        p = (M-1)*img(y, x);
        q = floor((M-1)*img(y, x));
        if rand>p-q
            pix(y, x) = q;
        else
            pix(y, x) = q + 1;
        end
    end
end

figure;
image(uint8(pix));
colormap(gray(M));
truesize;
pixx = pix;

pix = zeros(Y, X);
for y = 1:Y
    for x = 1:X
        p = (M-1)*img(y, x);
        q = floor((M-1)*img(y, x));
        if rand>p-q
            v = q;
        else
            v = q + 1;
        end
        m = round(randn/2);
        while abs(m)>1
            m = round(randn/2);
        end
        if v>0 && v<M-1
            pix(y, x) = v + m;
        else
            pix(y, x) = v;
        end
    end
end

figure;
image(uint8(pix));
colormap(gray(M));
truesize;

figure; hold on;
plot(mean(pix, 1));
plot(mean(pixx, 1));