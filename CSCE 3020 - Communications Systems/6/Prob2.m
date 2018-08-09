clc; clear all; close all;

%s = -100:.01:100;
syms s;

H = 1./((s.^2) - (3.*s) +1);
h = ilaplace(H);
X1 = 1./s;
X2 = 1./(s.^2);

%part i

Y1 = H.*X1;
y1 = ilaplace(Y1);
fplot(h,[-2 5],'color','r'); hold on;
fplot(y1,[-2 5],'color','b');
title('Unit Step Impulse');

%part ii
Y2 = H.*X2;
y2 = ilaplace(Y2);
figure(2);
fplot(h,[-2 5],'color','r'); hold on;
fplot(y2,[-2 5],'color','b');
title('Ramp Impulse');



