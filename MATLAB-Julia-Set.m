N = 256; B = 0.75;
X = linspace(-1, 1, N);
Y = linspace(-1, 1, N);
while true
    B = B*sqrt(sqrt(0.75))
    y0 = 0.636;
    x0 = 0.107;
    X = linspace(x0-B, x0+B, N);
    Y = linspace(y0-B, y0+B, N);
    for y = 1:N
        for x = 1:N
            % z = 0.0;                  % Mandelbrot
            % z0 = X(x) + Y(y)*j;       % Mandelbrot
            z = X(x) + Y(y)*j;          % Julia
            z0 = 0.5 + 0.5*j;           % Julia
            iteration = 0;
            while abs(z) < 2 && iteration < 128
                z = z^2 + z0;
                iteration = iteration + 1;
            end
            s(x, y) = log(iteration);
        end
    end
    imagesc(s); drawnow;
end
