clear all
close all
t=0:.001:0.4;
x=10*sinc(100*t);
subplot(2,2,1)
plot(t,x);
grid on;

t2=0:1/100:0.4;
x2=10*sinc(100*t2);
subplot(2,2,2)
stem(0:length(x2)-1,x2);
title('Nyquist rate');
grid on;

t3=0:1/200:0.4;
x3=10*sinc(100*t3);
subplot(2,2,3)
stem(0:length(x3)-1,x3);
title('2 * Nyquist rate');
grid on;

t4=0:1/50:0.4;
x4=10*sinc(100*t4);
subplot(2,2,4)
stem(0:length(x4)-1,x4);
title('1/2 * Nyquist rate');
grid on;
