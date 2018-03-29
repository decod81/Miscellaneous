map = gray(256);
phi = zeros(256, 256);
dphi = zeros(256, 256);
ddphi = zeros(256, 256);
rho = zeros(256, 256);
[x y] = meshgrid(linspace(-1, 1, 256));
for t = 0:256
  dphi(round(160 - t*0.2), 160) = 15*sin(t/10);
  rho = exp(-4000*((x+0.002*t).^2 + y.^2));
  ddphi = rho + del2(phi);
  dphi = dphi + ddphi;
  phi = phi + dphi;
  cla;
  surface(phi, 'edgecolor', 'none');
  colormap(map);
  axis image;
  shading interp;
  caxis([-20 20]);
  drawnow;
end
phi = phi - min(min(phi));
imwrite(1.25*phi/max(max(phi)), 'MATLAB-Waves-Screenshot.png');