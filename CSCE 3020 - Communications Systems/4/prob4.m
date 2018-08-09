clc; clear all; close all;

t = 0: 1e-8 : 160e-6;
kf = 2e5*pi;
m = 1;
k = 1;
c = cos(2e6*pi.*t);

%Message Signal
x = x(t);
subplot(4,3,1);
plot(t,x);
title('Message Signal');

X = fft(x);
subplot(4,3,2);
plot(t,abs(X));
title('Frequency Response');

subplot(4,3,3);
plot(t, angle(X));
title('Phase Response');

%DSB-SC
sc = x .*c;
subplot(4,3,4);
plot(t, sc);
title('DSB-SC');

SC = fft(sc);
subplot(4,3,5);
plot(t,abs(SC));
title('Frequency Response');

subplot(4,3,6);
plot(t, angle(SC));
title('Phase Response');

%DSB-TC
tc = (k + m.*x).*c;
subplot(4,3,7);
plot(t, tc);
title('DSB-TC');

TC = fft(tc);
subplot(4,3,8);
plot(t,abs(TC));
title('Frequency Response');

subplot(4,3,9);
plot(t, angle(TC));
title('Phase Response');

%Frequency Modulation
n = 4; %%number of periods
x_integral = n*(60e-6); %%60e-6 is the integration of x(t) over one period
fmtheta = kf.*x_integral;
fm = cos(2e6*pi.*t + fmtheta);

subplot(4,3,10)
plot(t,fm)
title('FM Time Domain');

FM = fft(fm);
subplot(4,3,11)
plot(t,abs(FM));
title('FM Frequency Domain');

subplot(4,3,12)
plot(t, angle(FM));
title('FM Phase Response');