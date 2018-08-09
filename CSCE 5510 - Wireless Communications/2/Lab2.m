clc; clear all; close all;

% ASK
t = 0:.001:10;
data = [1,0,0,1,1,0,0,1];
Sask = 5*cos(1000*pi*t);
sSampled = zeros(1, length(data));

for i = 1:length(data)
    if data(i) == 0
        sSampled(i) = 0;
    else
        sSampled(i) = Sask(i);
    end
end

subplot(3,1,1);
stem(sSampled);
xlabel('Time (ms)')
ylabel('Amplitude')
title('ASK')
xt = get(gca, 'XTick');  % 'XTick' Values
set(gca, 'XTick', xt, 'XTickLabel', (xt-1))

% MFSK
data2 = ["10","01","01","01","10","10","01","11"];
fc = 5e3;
fd = 500;
M = 4;

f1 = fc + (2*1-1-M)*fd; %3500 Hz
f2 = fc + (2*2-1-M)*fd; %4500 Hz
f3 = fc + (2*3-1-M)*fd; %5500 Hz
f4 = fc + (2*4-1-M)*fd; %6500 Hz

mfsk1 = cos(2*pi*f1*t); %00
mfsk2 = cos(2*pi*f2*t); %01
mfsk3 = cos(2*pi*f3*t); %10
mfsk4 = cos(2*pi*f4*t); %11

sSampled2 = zeros(1, length(data2));
freq = zeros(1, length(data2));

for i = 1:length(data2)
   if strcmp(data2(i), "00")
     sSampled2(i) = mfsk1(i);
     freq(i) = f1;
   elseif strcmp(data2(i), "01")
     sSampled2(i) = mfsk2(i); 
     freq(i) = f2;
   elseif strcmp(data2(i), "10")
     sSampled2(i) = mfsk3(i);
     freq(i) = f3;
   elseif strcmp(data2(i), "11")
     sSampled2(i) = mfsk4(i);
     freq(i) = f4;
   end
end

subplot(3,2,3);
stem(sSampled2);
xlabel('Time (ms)')
ylabel('Amplitude')
title('MFSK')
xt = get(gca, 'XTick');  % 'XTick' Values
set(gca, 'XTick', xt, 'XTickLabel', (xt-1))

subplot(3,2,4);
stem(freq);
xlabel('Time (ms)')
ylabel('Frequency (Hz)')
ylim([f1,f4]);
title('MFSK')
xt = get(gca, 'XTick');  % 'XTick' Values
set(gca, 'XTick', xt, 'XTickLabel', (xt-1))

% QPSK

s11 = cos(2*pi*fc*t + (pi/4));
s01 = cos(2*pi*fc*t + (3*pi/4));
s00 = cos(2*pi*fc*t - (3*pi/4));
s10 = cos(2*pi*fc*t - (pi/4));

sSampled3 = zeros(1, length(data2));
phase = zeros(1, length(data2));

for i = 1:length(data2)
   if strcmp(data2(i), "00")
     sSampled3(i) = s00(i);
     phase(i) = -3*pi/4;
   elseif strcmp(data2(i), "01")
     sSampled3(i) = s01(i); 
     phase(i) = 3*pi/4;
   elseif strcmp(data2(i), "10")
     sSampled3(i) = s10(i);
     phase(i) = -pi/4;
   elseif strcmp(data2(i), "11")
     sSampled3(i) = s11(i);
     phase(i) = pi/4;
   end
end

subplot(3,2,5);
stem(sSampled3);
xlabel('Time (ms)')
ylabel('Amplitude')
title('QPSK')
xt = get(gca, 'XTick');  % 'XTick' Values
set(gca, 'XTick', xt, 'XTickLabel', (xt-1))

subplot(3,2,6);
stem(phase);
xlabel('Time (ms)')
ylabel('Phase')
title('QPSK')
xt = get(gca, 'XTick');  % 'XTick' Values
set(gca, 'XTick', xt, 'XTickLabel', (xt-1))
