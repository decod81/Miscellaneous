clear all

A = imread('MATLAB-Cylinder-Mapping-Stone.png');
N = length(A);

% for y = 1:N
%     for x = 1:N
%         A(x, y) = y/2;
%     end
% end

z = zeros(N, N);
for y = 1:N
    for x = 1:N
        Z = (x - N/2) + (y - N/2)*j;
        z(x, y) = A( round(N/2 + abs(Z)/sqrt(2)), round(N/2 + (N/2-1)*angle(Z)/pi) );
    end
end
figure
imagesc(z);
colormap(gray(256));
imwrite(z/max(max(z)), 'MATLAB-Cylinder-Mapping-Screenshot.png');
truesize;
