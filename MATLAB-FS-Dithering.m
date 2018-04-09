%% multicolor Floyd-Steinberg
% clear all
close all

pixel = imread('david.png');
% pixel = imread('color.png');
% pixel = pixel(:, :, 3);
figure; image(pixel); colormap(gray(256)); truesize;
pixel = double(pixel)/255;

% for x = 1:256
%     pixel(x, :) = linspace(0, 1, 1024);
% end

N = 4;
l = linspace(0, 1, N)
for x = 1:N-1
    pixeli{x} = N*(pixel>l(x)).*(pixel<=l(x+1)).*pixel;
end

[Y X] = size(pixel);
for z = 1:N-1
    for y = 1+1:Y-1
       for x = 1+1:X-1
          oldpixel = pixeli{z}(y, x);
          newpixel = round(oldpixel);
          pixeli{z}(y, x) = newpixel;
          quant_error = oldpixel - newpixel;
          pixeli{z}(y,   x+1) = pixeli{z}(y,   x+1) + quant_error*7/16;
          pixeli{z}(y+1, x-1) = pixeli{z}(y+1, x-1) + quant_error*3/16;
          pixeli{z}(y+1, x)   = pixeli{z}(y+1, x)   + quant_error*5/16;
          pixeli{z}(y+1, x+1) = pixeli{z}(y+1, x+1) + quant_error*1/16;
       end
    end
    z
end

img = 0;
for z = 1:N-1
    img = img + pixeli{z};
end
figure;
image(uint8(256/N*img));
colormap(gray(256));
truesize;

% imwrite(uint8(256/N*img), 'david_multifs.png');
% B=uint8(256/N*img)

% img(:,:,1) = R;
% img(:,:,2) = G;
% img(:,:,3) = B;
% imagesc(uint8(4/N*img));
% truesize;
% imwrite(uint8(4/N*img), 'color_3x6b.png');