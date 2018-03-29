map   = gray(256);
N     = 256;
phi   = zeros(N, N);
dphi  = zeros(N, N);
ddphi = zeros(N, N);
rho   = zeros(N, N);
[x y] = meshgrid(linspace(-1, 1, N));
f     = 0.01;
A     = 0.05;
for t = 1:N
    x1 = cos(2*pi*f*t)*A;
    y1 = sin(2*pi*f*t)*A;
    x2 = -cos(2*pi*f*t)*A;
    y2 = -sin(2*pi*f*t)*A;
    rho = exp(-2000*((x+x1).^2+(y+y1).^2)) + exp(-2000*((x+x2).^2+(y+y2).^2));

    ddphi = rho + del2(phi);
    dphi = dphi + ddphi;

     phi(1, :)   =   phi(2, :);
    dphi(1, :)   = -dphi(2, :);
     phi(end, :) =   phi(end-1, :);
    dphi(end, :) = -dphi(end-1, :);
     phi(:, 1)   =   phi(:, 2);
    dphi(:, 1)   = -dphi(:, 2);
     phi(:, end) =   phi(:, end-1);
    dphi(:, end) = -dphi(:, end-1);

    phi = phi + dphi;

    cla;
    c = phi/max(max(phi));
    surface(c, 'edgecolor', 'none');
    colormap(map);
    caxis([0 1]);
    axis([1 N 1 N]);
    axis square;
    axis off;
    shading interp;
    drawnow;
    % imwrite(c, ['png/' num2str(t, '%04.f') '.png']);
    % ffmpeg -r 30 -f image2 -i %04d.png -vcodec libx264 -crf 2 output.mp4
end
imwrite(c/max(max(c)), 'MATLAB-Wave-Equation-Screenshot.png');