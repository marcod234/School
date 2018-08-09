clc; clear all; close all;

w = - pi : .001 : pi;
h = (.0593 -.0914*exp(-1i.*w) + .0593*exp(-1i*2.*w))./(1.257*exp(-1i.*w) + .467*exp(-1i*2.*w));

subplot(2,1,1);
plot(w,abs(h));
title('Magnitude');

subplot(2,1,2);
plot(w,angle(h));
title('phase');





