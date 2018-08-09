close all;

t = -3:.001:3;
x = 3*(sinc(4.*t)).^2;
c = 3*cos(800e3*pi.*t);
k = 1;
m = 1;

y = (k + m*x).*c; 
subplot(1,3,1)
plot(t,y)
title('DSB-TC Time Domain');

Y = fft(y);
subplot(1,3,2)
plot(t,abs(Y))
title('DSB-TC Frequency Domain');

subplot(1,3,3)
plot(t, angle(Y));
title('DSB-TC Phase Response');