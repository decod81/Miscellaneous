% Fermat primality test

one = java.math.BigInteger.ONE;
p = java.math.BigInteger('221');
n = p.subtract(java.math.BigInteger.ONE);

a = one.nextProbablePrime();
a.modPow(n, p) % a^n mod p

a = a.nextProbablePrime();
a.modPow(n, p) % this needs to be always one if p is prime
