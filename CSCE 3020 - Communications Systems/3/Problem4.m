clc; clear all; close all;

n = 0:1:100;
z = ones(1,101);
h = .9.^n .* ones(1,101); %filter
x = 3*cos(2*pi.*n/14); %input

H = fft(h,101);
X = fft(x,101);

Y = X.*H;
y = ifft(Y);

subplot(2,2,1);
plot(n,abs(H));
title('Filter Magnitude');

subplot(2,2,2);
plot(n, angle(H));
title('Filter Phase');

subplot (2,2,3);
plot(n, x);
title('Input');

subplot(2,2,4);
plot(n,y);
title('Output');
