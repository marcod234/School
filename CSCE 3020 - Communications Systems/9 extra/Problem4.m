clc; clear all; close all;

t = 0: 2*pi/100 : 2*pi;
m = .5;
k = 10;
c = cos(2*pi*800000.*t);
x = 3*cos(2*pi*1000.*t) + 2*cos(2*pi*2000*t);

%Message Signal
subplot(3,3,1);
plot(t,x);
title('Message Signal');

X = fft(x);
subplot(3,3,2);
plot(t,abs(X));
title('Frequency Response');

subplot(3,3,3);
plot(t, angle(X));
title('Phase Response');

%Carrier Signal
subplot(3,3,4);
plot(t, c);
title('Carrier Signal');

C = fft(c);
subplot(3,3,5);
plot(t,abs(C));
title('Frequency Response');

subplot(3,3,6);
plot(t, angle(C));
title('Phase Response');

%DSB-TC
tc = (k + m.*x).*c;
subplot(3,3,7);
plot(t, tc);
title('DSB-TC');

TC = fft(tc);
subplot(3,3,8);
plot(t,abs(TC));
title('Frequency Response');

subplot(3,3,9);
plot(t, angle(TC));
title('Phase Response');