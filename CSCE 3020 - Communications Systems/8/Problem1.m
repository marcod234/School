clc; clear all; close all;

syms s t n z w;
fs = 1000;
ts = 1/fs;
W = -2*pi: ts : 2*pi;

wc = 1;% change value of wc here
P1 = -1 *wc; 
Px = P1 + cos(pi/3); %real part of pole
Py = sin(pi/3); %imaginary part of pole
P2 = Px + (Py *1i);
P3 = Px - (Py *1i);

% deriving 3rd order Butterworth Filter
H = (P1/(P1 - s)) * (P2/(P2 - s)) * (P3/(P3 - s));
H = expand(H)

%impulse invariance method
htf = ilaplace(H); %take the inverse laplace of the transfer function
htf = subs(htf, t, ts*n); % substitute t with nTs
ztf = ztrans(htf); % take z transform
impulse = subs(ztf, z, exp(1i*w)) % substitute z with e^iw
zTF = matlabFunction(impulse); %convert symbolic equation to function
ZTF = zTF(W); %vectorize data
subplot(4,2,1); %plot
stem(W, abs(20*log10(ZTF)));
title('Impulse Invariant Magnitude Response (DB)');
subplot(4,2,2); %plot
stem(W, angle((ZTF)));
title('Impulse Invariant Phase Response');

%step invariance method
Ha= H/s;
ha = ilaplace(Ha);
hatf = subs(ha, t, ts*n);
zdtf = ztrans(hatf);
step = subs(zdtf, z, exp(1i*w))
zdTF = matlabFunction(step); 
ZDTF = zdTF(W);
subplot(4,2,3);
stem(W, abs(20*log10(ZDTF)));
title('Step Invariant Magnitude Response (DB)');
subplot(4,2,4); 
stem(W, angle((ZDTF)));
title('Step Invariant Phase Response');

%backward difference
Hbd = subs(H, s, ((z-1)/(z*ts)));
Hbd = simplify(Hbd);
BackDiff = subs(Hbd, z, exp(1i*w))
bd = matlabFunction(BackDiff); 
BD = bd(W);
subplot(4,2,5);
stem(W, abs(20*log10(BD)));
title('Backward Difference Magnitude Response (DB)');
subplot(4,2,6); 
stem(W, angle((BD)));
title('Backward Difference Phase Response');

%forward difference
Hfd = subs(H, s, ((z-1)/ts));
Hfd = simplify(Hfd);
ForwDiff = subs(Hfd, z, exp(1i*w))
fd = matlabFunction(ForwDiff); 
FD = fd(W);
subplot(4,2,7);
stem(W, abs(20*log10(FD)));
title('Forward Difference Magnitude Response (DB)');
subplot(4,2,8); 
stem(W, angle((FD)));
title('Forward Difference Phase Response');

%bilinear
Hb = subs(H, s, ((2/ts)*((z-1)/(z+1))));
Hb = simplify(Hb);
Bl = subs(Hb, z, exp(1i*w))
b = matlabFunction(Bl); 
B = b(W);
figure(2); %new figure
subplot(3,2,1);
stem(W, abs(20*log10(B)));
title('Bilinear Magnitude Response (DB)');
subplot(3,2,2); 
stem(W, angle((B)));
title('Bilinear Phase Response');

%matched z tranform
%from http://www.mathworks.com/matlabcentral/fileexchange/27302-syms-to-tf-conversion?focused=5151036&tab=function
[symNum,symDen] = numden(H); %Get num and den of Symbolic TF
TFnum = sym2poly(symNum);   %Convert Symbolic num to polynomial
TFden = sym2poly(symDen);    %Convert Symbolic den to polynomial
[r,p,k] = residue(TFnum, TFden);

Hmz = (r(1,1)/(1 - exp(p(1,1)*ts)/z)) + (r(2,1)/(1 - exp(p(2,1)*ts)/z)) + (r(3,1)/(1 - exp(p(3,1)*ts)/z));
Hmz = expand(Hmz);
HMZ = subs(Hmz, z, exp(1i*w))
mz = matlabFunction(HMZ); 
MZ = mz(W);
subplot(3,2,3);
stem(W, abs(20*log10(MZ)));
title('Matched Z-Transorm Response (DB)');
subplot(3,2,4); 
stem(W, angle((MZ)));
title('Matched Z-Transorm Phase Response');

%direct substitution

Hds = (r(1,1)/(z - exp(p(1,1)*ts))) + (r(2,1)/(z - exp(p(2,1)*ts))) + (r(3,1)/(z - exp(p(3,1)*ts)));
Hds = expand(Hds);
HDS = subs(Hds, z, exp(1i*w))
ds = matlabFunction(HDS); 
DS = ds(W);
subplot(3,2,5);
stem(W, abs(20*log10(DS)));
title('Direct Substitution Response (DB)');
subplot(3,2,6); 
stem(W, angle((DS)));
title('Direct Substitution Phase Response');



