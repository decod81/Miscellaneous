%% make the hidden message visible
pic = imread('MATLAB-Steganography-Photo.jpg');
imagesc(log10(abs(fft2(pic(:, :, 1)))), [0 5]);
truesize;

%% ...and here's how it was made
pic = imread('org_photo.png');
MSG = imread('message.png');
R = fft2(pic(:, :, 1));
R = R + 15*double(MSG);
r = ifft2(R, 'symmetric');
pic(:, :, 1) = abs(r);
imwrite(pic, 'me.jpg', 'JPG');

%% read the hidden message from the cat-picture
clc
close all

pic = imread('MATLAB-Steganography-Cat.png');
imshow(pic);
sprintf(char(bin2dec(vec2mat(sprintf('%d', double(mod(pic(1:1702*8), 2))), 8))))

%% ...and here's how it was made
message = double('You ask me for a hamburger. My attempt to reciprocate is cut brutally short as my body experiences a sudden lack of electrons. Across a variety of hidden dimensions you are dismayed. John Lennon hands me an apple, but it slips through my fingers. I am reborn as an ocelot. You disapprove. A crack echoes through the universe in defiance of conventional physics as cosmological background noise shifts from randomness to a perfect A Flat. Children everywhere stop what they are doing and hum along in perfect pitch with the background radiation. Birds fall from the sky as the sun engulfs the earth. You hesitate momentarily before allowing yourself to assume the locus of all knowledge. Entropy crumbles as you peruse the information contained within the universe. A small library in Phoenix ceases to exist. You stumble under the weight of everythingness, Your mouth opens up to cry out, and collapses around your body before blinking you out of the spatial plane. You exist only within the fourth dimension. The fountainhead of all knowledge rolls along the ground and collides with a small dog. My head tastes sideways as spacetime is reestablished, you blink back into the corporeal world disoriented, only for me to hand you a hamburger as my body collapses under the strain of reconstitution. The universe has reasserted itself. A particular small dog is fed steak for the rest of its natural life. You die in a freak accident moments later, and you soul works at the returns desk for the Phoenix library. You disapprove. Your disapproval sends ripples through the inter-dimensional void between life and death. A small child begins to cry as he walks toward the stairway where his father stands.');

message_in_bin = dec2bin(message, 8);
A = imread('org_cat.png');
ss = size(message_in_bin);
message = message_in_bin';
for x = 1:ss(1)*ss(2)
    value = bin2dec(message(x));
    if mod(A(x), 2) ~= value
        if A(x) > 100
            A(x) = A(x) - 1;
        else
            A(x) = A(x) + 1;
        end
    end
end
imwrite(A, 'new_at.png', 'PNG');