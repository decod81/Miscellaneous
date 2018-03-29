clear all;
N = 64;
[x y] = meshgrid(linspace(-1, 1, N), linspace(-1, 1, N));
A = 1.4;
B = -0.2;
C = 1.2;
psi{1} = exp(-50*((x-0.5*A-B).^2 + (y-0.4*A).^2)).*exp(-j*( 20*x*C + 10*y*C));
psi{2} = exp(-50*((x+0.2*A-B).^2 + (y+0.3*A).^2)).*exp(-j*(-20*x*C - 10*y*C));
psi{3} = exp(-50*((x-0.4*A-B).^2 + (y+0.3*A).^2)).*exp(-j*( 10*x*C - 20*y*C));
v = fft2(0.3./(x.^2+y.^2+0.01));
for x = 1:3
    psi{x} = psi{x}/norm(psi{x});
end
in = 1;
for t = 0:15000
    if mod(t, 150)==0
        B = 0;
        for x = 1:3
            A = abs(psi{x}).^2;
            B = B + A/max(max(A));
        end
        figure(1);
        cla;
        imagesc(imresize(B, 4));
        truesize;
        drawnow;
        % imwrite(B, [num2str(in, '%04.f') '.png']);
        in = in + 1
    end
    for x = 1:3 % fast 2D convolution with fft
        V{x} = fftshift(ifft2(v.*fft2(conj(psi{x}).*psi{x})));
    end
    for x = 1:3 % separate wave functions
        W = zeros(N, N);
        for y = 1:3
            if y~=x
                W = W + V{y};
            end
        end
        psi{x} = psi{x} + 1e-3*j*(4*del2(psi{x}) - W.*psi{x});
    end
end
