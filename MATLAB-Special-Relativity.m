clear all
close all

N = 10;
v = 0.8;
gamma = sqrt(1-v^2) % "speed of time for the traveller"
ax = 0;
at = 0;
for x = 1:N
    at(x) = x/gamma;
    ax(x) = v*at(x);

end
t = linspace(0, N+1, 2^14);

figure;
hold on;

plot(t, t, 'linewidth', 2);
plot(v*t, t, 'linewidth', 2);
plot(ax, at, 'ok', 'linewidth', 2);

try
    for s = 1:N
        x = linspace(0, ax(s), 256);
        t = linspace(ax(s)+at(s), at(s), 256);
        plot(x, t, '--r', 'linewidth', 2);
        x = linspace(0, s*v/(1-v), 256);
        t = s+x;
        plot(x, t, '--b', 'linewidth', 2);
    end
end

legend('light', 'traveller', 'light clock', 'tick rays', 'tick rays', 'location', 'east');
xlabel('x');
ylabel('t');
axis([0 N+1 -1 N]);
axis([0 10 0 10]);
axis square;
box on;

axes('Position',[.55 .2 .22 .22])
plot([0 v v 0], [0 0 gamma 0], 'linewidth', 2);
axis([-0.1 1 -0.1 1]);
text(0.21, -0.12, '0.8');
text(0.87, 0.28, '0.6');
text(0.3, 0.38, '1');
axis square;
axis off;