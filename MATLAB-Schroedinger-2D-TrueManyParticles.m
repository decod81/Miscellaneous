% simulation of three particle Schroedinger equation
n   = 0.05; % naive numerical factor which helps to stabilize the simulation
N   = 20;
fs  = 40;
x10 = -0.4; y10 =  0.4; vx1 = 0; vy1 = 0;
x20 =  0.4; y20 =  0.4; vx2 = 0; vy2 = 0;
x30 =  0.0; y30 = -0.4; vx3 = 0; vy3 = 0;
psi = zeros(N, N, N, N, N, N, 'single');
V   = zeros(N, N, N, N, N, N, 'single');
[X1 Y1 X2 Y2 X3 Y3] = ndgrid(linspace(-1.0, 1.0, N));
V = 1./sqrt(n + (X1-X2).^2 + (Y1-Y2).^2).^2 + ...
    1./sqrt(n + (X1-X3).^2 + (Y1-Y3).^2).^2 + ...
    1./sqrt(n + (X2-X3).^2 + (Y2-Y3).^2).^2;
psi = exp(-fs*((X1+x10).^2 + (Y1+y10).^2 + ...
               (X2+x20).^2 + (Y2+y20).^2 + ...
               (X3+x30).^2 + (Y3+y30).^2)).* ...
      exp(-j*(vx1*X1 + vy1*Y1 + ...
              vx2*X2 + vy2*Y2 + ...
              vx3*X2 + vy3*Y2));
for t = 1:200
    PSI1 = squeeze(mean(mean(mean(mean(psi, 1), 2), 3), 4));
    PSI2 = squeeze(mean(mean(mean(mean(psi, 1), 2), 5), 6));
    PSI3 = squeeze(mean(mean(mean(mean(psi, 3), 4), 5), 6));
    a = abs(PSI1).^2; a = a/max(max(a));
    b = abs(PSI2).^2; b = b/max(max(b));
    c = abs(PSI3).^2; c = c/max(max(c));
    val = a + b + c;
    imwrite(val/max(max(val)), ['png/' num2str(t, '%04.f') '.png']);
    psi = psi + 0.005*j*(del2(psi) - V.*psi);

    % z = zeros(N, N, N, N, N, N); % below: another way to compute del2(psi)
    % z(2:N-1, :, :, :, :, :) = z(2:N-1, :, :, :, :, :) + diff(psi, 2, 1);
    % z(:, 2:N-1, :, :, :, :) = z(:, 2:N-1, :, :, :, :) + diff(psi, 2, 2);
    % z(:, :, 2:N-1, :, :, :) = z(:, :, 2:N-1, :, :, :) + diff(psi, 2, 3);
    % z(:, :, :, 2:N-1, :, :) = z(:, :, :, 2:N-1, :, :) + diff(psi, 2, 4);
    % z(:, :, :, :, 2:N-1, :) = z(:, :, :, :, 2:N-1, :) + diff(psi, 2, 5);
    % z(:, :, :, :, :, 2:N-1) = z(:, :, :, :, :, 2:N-1) + diff(psi, 2, 6);
end
