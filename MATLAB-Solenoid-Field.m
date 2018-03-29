%% Magnetic field solver for a solenoid
N = 10;         % Number of turns
R = 0.4;        % Radius of the coil
n = 2^9;        % Number of segments in the coil
I = 10000;      % Current
res = 0.0125;   % Spatial resolution

s = linspace(-1, 1, n);
x = R*sin(pi*N*s);
y = R*cos(pi*N*s);
z = R*s;

figure
plot3(x, y, z)

h = figure;
field = zeros(length([0:res:2*R]), length([0:res:2*R]));
for z0 = 0:res:R*2
    for x0 = 0:res:R*2
        y0 = 0;
        B = 0;
        u0 = 4*pi*1e-7;
        for a = 2:length(s)
            dl = [x(a)-x(a-1) y(a)-y(a-1) z(a)-z(a-1)];
            r = [x(a)-x0 y(a)-y0 z(a)-z0];
            B = B + cross(dl, r/norm(r))/norm(r)^2;
        end
        field(round(1+z0/res), round(1+x0/res)) = u0*I/4/pi*B(3);
    end
    figure(h)
    clf
    surface(linspace(0, 2*R, length(field)), linspace(0, 2*R, length(field)), field, 'edgecolor', 'none')
    colorbar
    drawnow
end
colormap(parula(256));
shading interp;