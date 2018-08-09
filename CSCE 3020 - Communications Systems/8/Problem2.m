clc; clear all; close all
syms s t n z w x;
fs = 10;
ts = 1/fs;
W = logspace(-2*pi, 2*pi);
omega = -2*pi : ts : 2*pi;

%2a seventh order normalized low pass elliptic filter - Analog
[ze, pe, ke] = ellipap(7, .3, .5);
[nume, dene] = tfdata(zpk(ze,pe,ke), 'v');
elliptic = tf(nume, dene)
figure(1);
freqs(nume, dene, W);
[re,pe,ke] = residue(nume, dene);

%2b eighth order normalized low pass Chebyshev Type 1 - Analog
[zc,pc,kc] = cheb1ap(8,0.3);
[numc, denc] = tfdata(zpk(zc,pc,kc), 'v');
chebyshev = tf(numc, denc)
figure(2);
freqs(numc, denc, W);
[rc,pc,kc] = residue(numc, denc);

%2a seventh order normalized low pass elliptic filter - Digital
H = re(1,1)/(s - pe(1,1)) + re(2,1)/(s - pe(2,1)) + re(3,1)/(s - pe(3,1)) ...
    + re(4,1)/(s - pe(4,1)) + re(5,1)/(s - pe(5,1)) + re(6,1)/(s - pe(6,1)) ...
    + re(7,1)/(s - pe(7,1));

h = ilaplace(H);
h = subs(h, t, ts*n);
ztf = ztrans(h);
impulseE = subs(ztf, z, exp(1i*w))
zTF = matlabFunction(impulseE);
ZTF = zTF(omega);
figure(3);
subplot(2,2,1);
stem(omega, abs(20*log10(ZTF)));
title('Chebyshev Impulse Invariant Magnitude Response');
subplot(2,2,2);
stem(omega, angle(ZTF));
title('Chebyshev Impulse Invariant Frequency Response');

%2b eighth order normalized low pass Chebyshev Type 1 - Digital
H = rc(1,1)/(s - pc(1,1)) + rc(2,1)/(s - pc(2,1)) + rc(3,1)/(s - pc(3,1)) ...
    + rc(4,1)/(s - pc(4,1)) + rc(5,1)/(s - pc(5,1)) + rc(6,1)/(s - pc(6,1)) ...
    + rc(7,1)/(s - pc(7,1)) + rc(8,1)/(s - pc(8,1));

h = ilaplace(H);
h = subs(h, t, ts*n);
ztf = ztrans(h);
impulseC = subs(ztf, z, exp(1i*w))
zTF = matlabFunction(impulseC);
ZTF = zTF(omega);
figure(3);
subplot(2,2,3);
stem(omega, abs(20*log10(ZTF)));
title('Chebyshev Impulse Invariant Magnitude Response');
subplot(2,2,4);
stem(omega, angle(ZTF));
title('Chebyshev Impulse Invariant Frequency Response');