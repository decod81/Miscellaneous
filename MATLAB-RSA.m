% Simple RSA cryptosystem testing in matlab

one = java.math.BigInteger.ONE;
p = one.probablePrime(256, java.util.Random);
q = one.probablePrime(256, java.util.Random);


n = p.multiply(q); % first part of the public key (n, e)
tmp0 = p.subtract(one);
tmp1 = q.subtract(one);
totient = tmp0.multiply(tmp1);
tmp0 = java.math.BigInteger('2');
tmp1 = totient.divide(tmp0);
e = tmp1.nextProbablePrime(); % second part of the public key (n, e)
d = e.modInverse(totient); % the private key

m = java.math.BigInteger('123321243425345'); % message

c = m.modPow(e, n) % encryption
m = c.modPow(d, n) % decryption
