%% Floyd-Steinberg dithering
clear all
close all

M = 4;
pixel = imread('david.png');
figure; image(pixel); colormap(gray(256)); truesize;
pixel = (M-1)*double(pixel)/255;

[Y X] = size(pixel);
for y = 2:Y-1
    for x = 2:X-1
        oldpixel = pixel(y, x);
        newpixel = ceil(oldpixel);
        pixel(y, x) = newpixel;
        quant_error = oldpixel - newpixel;
        pixel(y,   x+1) = pixel(y,   x+1) + quant_error*7/16;
        pixel(y+1, x-1) = pixel(y+1, x-1) + quant_error*3/16;
        pixel(y+1, x)   = pixel(y+1, x)   + quant_error*5/16;
        pixel(y+1, x+1) = pixel(y+1, x+1) + quant_error*1/16;
    end
end

figure;
image(uint8(255*pixel/(M-1)));
colormap(gray(256));
truesize;
imwrite(uint8(255*pixel/(M-1)), 'MATLAB-FS-dithering-David.png'); 