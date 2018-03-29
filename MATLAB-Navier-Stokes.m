clear all;
close all;

N  = 128;
p  = zeros(N, N);
px = zeros(N, N);
py = zeros(N, N);
u  = zeros(N, N);
v  = zeros(N, N);
us = zeros(N, N);
vs = zeros(N, N);
for t = 0:1500
    u(N/2-5:N/2+5, 0.7*N-5:0.7*N+5) = 0.15;
    v(N/2-5:N/2+5, 0.7*N-5:0.7*N+5) = 0;

    % edges make it unstable so get rid of them
    u(1, :) = 0; u(end, :) = 0; u(:, 1) = 0; u(:, end) = 0;
    v(1, :) = 0; v(end, :) = 0; v(:, 1) = 0; v(:, end) = 0;
    p(1, :) = 0; p(end, :) = 0; p(:, 1) = 0; p(:, end) = 0;

    % solve intermediate velocity u*
    [ux uy] = gradient(u);
    [vx vy] = gradient(v);
    us = u + (u.*ux + v.*uy + del2(u)/20 - px/20 + 0.00000);
    vs = v + (u.*vx + v.*vy + del2(v)/20 - py/20 + 0.00005);

    % solve poisson equation (approximate a little and it stabilizes)
    p = p + del2(p) - divergence(us, vs)/4;

    % projection method
    [px py] = gradient(p);
    u = us - px;
    v = vs - py;

    cla;
    surface(sqrt(u.^2 + v.^2), 'edgecolor', 'none');
    caxis([0 0.17]);
    axis square;
    shading interp;
    drawnow;
end
