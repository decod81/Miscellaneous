% magnetic field of a current loop
N = 48;
n = 2^6;
s = linspace(-1, 1, n);
x = 0.1*sin(pi*s);
y = 0.1*cos(pi*s);
z = 0*s;
d = linspace(-0.5, 0.5, N);
u = zeros(N, N, N);
v = zeros(N, N, N);
w = zeros(N, N, N);
for x0 = 1:N
    for y0 = 1:N
        for z0 = 1:N
            B = 0;
            for a = 2:length(s)
                dl = [x(a)-x(a-1) y(a)-y(a-1) z(a)-z(a-1)];
                r  = [x(a)-d(x0)  y(a)-d(y0)  z(a)-d(z0)];
                r0 = r/norm(r);
                B  = B + 1e-7*cross(dl, r0)/norm(r)^2;
            end
            u(x0, y0, z0) = B(1);
            v(x0, y0, z0) = B(2);
            w(x0, y0, z0) = B(3);
        end
    end
    x0
end
[x y z] = ndgrid(d, d, d);
A = (u.^2 + v.^2 + w.^2).^0.3; % scaling for visibility
a = sqrt(u.^2 + v.^2 + w.^2);
quiver3(x, y, z, A.*u./a, A.*v./a, A.*w./a);
view(40, 15);
axis([-1 1 -1 1 -1 1]*0.2);
axis square;
