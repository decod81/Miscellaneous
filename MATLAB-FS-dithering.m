clear all
close all

M = 4;
pixel = imread('color.png');
figure; image(pixel); truesize;

for z = 1:3
    pix{z} = (M-1)*double(pixel(:, :, z))/255;
    [Y X] = size(pix{z});
    for y = 2:Y-1
        for x = 2:X-1
            oldpixel = pix{z}(y, x);
            newpixel = round(oldpixel);
            pix{z}(y, x) = newpixel;
            quant_error = oldpixel - newpixel;
            pix{z}(y,   x+1) = pix{z}(y,   x+1) + quant_error*7/16;
            pix{z}(y+1, x-1) = pix{z}(y+1, x-1) + quant_error*3/16;
            pix{z}(y+1, x)   = pix{z}(y+1, x)   + quant_error*5/16;
            pix{z}(y+1, x+1) = pix{z}(y+1, x+1) + quant_error*1/16;
        end
    end
    img(:, :, z) = pix{z};
end
figure;
image(uint8(255*img/(M-1)));
truesize;
% imwrite(uint8(255*img/(M-1)), 'colors.png'); 