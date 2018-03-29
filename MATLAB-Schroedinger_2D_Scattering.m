N = 256;
[x y] = meshgrid(linspace(-1, 1, N), linspace(-1, 1, N));
psi = exp(-10*((x+0.5).^2 + (y+0.5).^2)).*exp(-j*(-50*x -50*y));
psi = psi/norm(psi);
V = zeros(N, N);
V(N/2-20:N/2-10, N/2:N/2+20) = 1;
V(N/2+10:N/2+20, N/2-10:N/2+10) = 1;
S = conv2(diff([0 0 1 0 0], 2), eye(N)); % del2 matrix form
for n = 1:N % H = V - del2
    Ex{n} = expm(-j*(diag(V(n, :)) - S(:, 2:end-1)));
    Ey{n} = expm(-j*(diag(V(:, n)) - S(:, 2:end-1)));
end
for t = 0:150 % time evolution psi(t+1) = psi(t)*exp(-j*H)
    for n = 1:N
        psi(:, n) = Ex{n}*psi(:, n);
    end
    for n = 1:N
        psi(n, :) = psi(n, :)*Ey{n};
    end
    A = abs(psi).^2/4.5e-4;
    cla;
    imagesc(A);
    caxis([0 1]);
    truesize;
    drawnow;
    % imwrite(imresize(A, 0.5), [num2str(t+1, '%04.f') '.png']);
    % ffmpeg -r 30 -f image2 -i %04d.png -vcodec libx264 -crf 25 -pix_fmt yuv420p test.mp4
    % convert -resize 50% -layers Optimize -delay 3 *.png test.gif
end
