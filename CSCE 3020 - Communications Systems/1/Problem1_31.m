clear all
close all
t=0:.001:0.4;
x=3*cos(20*pi*t) - 2*sin(30*pi*t);
subplot(3,2,1)
plot(t,x);
grid on;

t2=0:1/120:0.4;
x2=3*cos(20*pi*t2) - 2*sin(30*pi*t2);
subplot(3,2,2)
stem(0:length(x2)-1,x2);
grid on;

t3=0:1/60:0.4;
x3=3*cos(20*pi*t3) - 2*sin(30*pi*t3);
subplot(3,2,3)
stem(0:length(x3)-1,x3);
grid on;

t4=0:1/30:0.4;
x4=3*cos(20*pi*t4) - 2*sin(30*pi*t4);
subplot(3,2,4)
stem(0:length(x3)-1,x3);
grid on;

t5=0:1/15:0.4;
x5=3*cos(20*pi*t5) - 2*sin(30*pi*t5);
subplot(3,2,5)
stem(0:length(x5)-1,x5);
grid on;