a = java.math.BigInteger('2988348162058574136915891421498819466320163312926952423791023078876139');
b = java.math.BigInteger('2351399303373464486466122544523690094744975233415544072992656881240319');
m = java.math.BigInteger('87814412832655818143772433337418883492663173730772486450699007318453048545183');

ZERO = java.math.BigInteger.ZERO;
ONE = java.math.BigInteger.ONE;
s = ONE;
t = a;
u = b;

while u.equals(ZERO) == 0
    if u.and(ONE).equals(ONE) == 1
        s = s.multiply(t).mod(m);
    end
    u = u.shiftRight(1)
    t = t.multiply(t).mod(m);
end
a.modPow(b, m)
s
