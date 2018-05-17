close all
t = [-10:10];
v = 0.5;
gamma = 1/sqrt(1-v^2);
plot(gamma*v*t, gamma*t, '-or'); hold on;
plot(gamma*t, gamma*v*t, '-ob'); hold on;
t = linspace(-5, 5, 1001);
for a = [1/4 1/3 1/2 1]
    x = cosh(a*t)/a;
    y = sinh(a*t)/a;
    plot(x, y, 'k');
    plot(y, x, 'k');
    plot(-x, -y, 'k');
    plot(y, -x, 'k');
end
legend('time in another frame', 'space in another frame', 'const. acceleration', 'location', 'northwest');
axis(5*[-1 1 -1 1]);
ax = gca;
ax.XTick = [-5:5];
ax.YTick = [-5:5];
grid on;
axis square;
xlabel('space');
ylabel('time');