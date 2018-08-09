%Marco Duarte
clc; clear all; close all;
t=0:.0001:.3; %time
t1=0:.001:.3; %sampling time

y1 = sin(300*pi*t); %signals
y2 = cos(200*pi*t);
y3 = sin(300*pi*t) + cos(150*pi*t);

%%plot and sample y1
subplot(2,1,1);
plot(y1);
xlabel('Time (ms)');
xlim([0 3000])
ylabel('Amplitude');
title('y1 = sin(300*pi*t)');
xt = get(gca, 'XTick');  % 'XTick' Values
set(gca, 'XTick', xt, 'XTickLabel', xt/10)

y1s = sin(300*pi*t1); %sample y1
subplot (2,1,2);
stem(y1s);
xlabel('Time (ms)');
xlim([0 300])
ylabel('Amplitude');
title('y1 = sin(300*pi*t) Sampled');

%%plot y2
figure
subplot(2,1,1);
plot(y2);
xlabel('Time (ms)');
xlim([0 3000])
ylabel('Amplitude');
title('y2 = cos(200*pi*t)');
xt = get(gca, 'XTick');  % 'XTick' Values
set(gca, 'XTick', xt, 'XTickLabel', xt/10)

y2s = cos(200*pi*t1); %sample y2
subplot (2,1,2);
stem(y2s);
xlabel('Time (ms)');
xlim([0 300])
ylabel('Amplitude');
title('y2 = cos(200*pi*t) Sampled');

%%plot y3
figure
subplot(2,1,1);
plot(y3);
xlabel('Time (ms)');
xlim([0 3000])
ylabel('Amplitude');
title('y3 = sin(300*pi*t) + cos(150*pi*t)');
xt = get(gca, 'XTick');  % 'XTick' Values
set(gca, 'XTick', xt, 'XTickLabel', xt/10)

y3s =sin(300*pi*t1) + cos(150*pi*t1); %sample y3
subplot (2,1,2);
stem(y3s);
xlabel('Time (ms)');
xlim([0 300])
ylabel('Amplitude');
title('y3 = sin(300*pi*t) + cos(150*pi*t) Sampled');

%tdm of signals
for i = 1:300 %place each sample one after the other
    tdm(1, 3*i - 2) = y1s(i);
    tdm(1, 3*i - 1) = y2s(i);
    tdm(1, 3*i)     = y3s(i);
end

%plot tdm
figure
stem(tdm)
xlabel('Time (ms)');
ylabel('Amplitude');
title('TDM');

%recover signals
for i = 1:300
    y1r(1,i) = tdm(3*i - 2);
    y2r(1,i) = tdm(3*i - 1);
    y3r(1,i) = tdm(3*i);
end

%plot recovered signals
figure
subplot (3,1,1);
stem(y1r);
xlabel('Time (ms)');
xlim([0 300])
ylabel('Amplitude');
title('y1 = sin(300*pi*t) Recovered');

subplot (3,1,2);
stem(y2r);
xlabel('Time (ms)');
xlim([0 300])
ylabel('Amplitude');
title('y2 = cos(200*pi*t) Recovered');

subplot (3,1,3);
stem(y3r);
xlabel('Time (ms)');
xlim([0 300])
ylabel('Amplitude');
title('y3 = sin(300*pi*t) + cos(150*pi*t) Recovered');



