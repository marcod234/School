clear all;
close all;
t=-5:.001:5;
x=80*(sinc(4*t + 16) + sinc(4*t + 8) + sinc(4*t) + sinc(4*t - 8) + sinc(4*t - 16));
figure(1);
subplot(2,1,1)
plot(t,x);
grid on;

subplot(2,1,2)
plot(t,phase(x));
grid on;

x2 = 20*(sinc(4*t + 4) + sinc(4*t + 2) + sinc(4*t) + sinc(4*t - 2) + sinc(4*t - 4));
figure(2);
subplot(2,1,1)
plot(t,x2);
grid on;

subplot(2,1,2)
plot(t,phase(x2));
grid on;

