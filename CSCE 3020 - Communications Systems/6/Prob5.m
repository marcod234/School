clc; clear all; close all; 

%part i
num = [0 0 1 3]; %s+3
den = [1 1 9 9]; %s^3 + s^2 + 9s + 9

[r, p, k] = residue(num, den)

H = tf(num, den)

%part ii
rlocus(H);

%part iii
H2 = tf(3,1)

feedback(H,H2)
