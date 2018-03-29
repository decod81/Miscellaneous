n = java.math.BigInteger(vec2mat(dec2bin(double('Abibliophobia'),7),1)',2)
char(bin2dec(vec2mat(char(n.toString(2)),7))')

% r1 = javaObject('java.math.BigInteger', '1272217067541950449850037473')
% r2 = javaMethod('toString', r1, 2)
% r3 = char(r2)
% r4 = vec2mat(r3, 7)
% r5 = bin2dec(r4)
% r6 = transpose(r5)
% r7 = char(r6)

% a = javaObject('java.math.BigInteger', '1272217067541950449850037473')
% b = javaMethod('toString', a, 2)
% char(bin2dec(reshape(b, 7, length(b)/7)')')
