clear all
close all
clc

N = 256;
M = 2^19;
[x, y] = meshgrid(linspace(-1, 1, N), linspace(-1, 1, N));
z = -1./(0.05+x.^2+y.^2);
for n = 1:M
    X(n) = 0.0+randn/16;
    Y(n) = 0.5+randn/16;
    vx(n) = 4 + randn/16;
    vy(n) = randn/32;
%     X(n) = rand-0.5;
%     Y(n) = rand-0.5;
%     vx(n) = rand-0.5;
%     vy(n) = rand-0.5;
end
dt = 0.002;
[px, py] = gradient(z);

for t = 1:2300
    m = zeros(N, N);
    for n = 1:M
        xx = round(N/2+0.7*N/2*Y(n)+randn/8);
        yy = round(N/2+0.7*N/2*X(n)+randn/8);
%         ppx = interp2(x, y, px, Y(n), X(n));
%         ppy = interp2(x, y, py, Y(n), X(n));
        vx(n) = vx(n) - px(xx, yy);
        vy(n) = vy(n) - py(xx, yy);
%         vx(n) = vx(n) - ppx;
%         vx(n) = vx(n) - ppy;
        X(n) = X(n) + vx(n)*dt;
        Y(n) = Y(n) + vy(n)*dt;
        m(xx, yy) = m(xx, yy) + 1;
    end

    figure(1);
    cla; hold on;
    surface(x, y, m, 'edgecolor', 'none');
    caxis([0 200]);
    axis square;
    box on;
    title(num2str(t));
    drawnow;
end
figure;
% axis([-0.6 0.6 -0.6 0.6]);
imagesc(m);
colormap(gray(256));
axis off;
caxis([0 200]);
truesize;
% shading interp;