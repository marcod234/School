clc; clear all; close all;

t = -3:.001:3;
x = 2*cos(3000*pi.*t);
c = cos(3e6*pi.*t);
kp = .001;
kf = 2e5*pi;

%Phase Modulation
pmtheta = kp.*x;
pm = cos(3e6*pi.*t + pmtheta);

subplot(2,3,1)
plot(t,pm)
title('PM Time Domain');

PM = fft(pm);
subplot(2,3,2)
plot(t,abs(PM))
title('PM Frequency Domain');

subplot(2,3,3)
plot(t, angle(PM));
title('PM Phase Response');

%Frequency Modulation
x_integral = 2*sin(3000*pi.*t)/3000;
fmtheta = kf.*x_integral;
fm = cos(3e6*pi.*t + fmtheta);

subplot(2,3,4)
plot(t,fm)
title('FM Time Domain');

FM = fft(fm)
subplot(2,3,5)
plot(t,abs(FM));
title('FM Frequency Domain');

subplot(2,3,6)
plot(t, angle(FM));
title('FM Phase Response');