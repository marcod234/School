clc; clear all; close all;

t = 0:.00001:.003; %time
t1 = 0:.0003:.003; %sampling time

%Signals
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
y1 = sin(3000*pi*t);
y2 = cos(3000*pi*t);
y3 = sin(3000*pi*t) + cos(3000*pi*t);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Sampled Signals
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
y1s = sin(3000*pi*t1);
y2s = cos(3000*pi*t1);
y3s = sin(3000*pi*t1) + cos(3000*pi*t1);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%plotting signals
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
subplot(2, 2, 1);
plot(t, y1);
title('y1 = sin(3000*pi*t)');

subplot(2, 2, 2);
plot(t, y2);
title('y2 = cos(3000*pi*t)');

subplot(2, 1, 2);
plot(t, y3);
title('y3 = sin(3000*pi*t) + cos(3000*pi*t)');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%https://www.mathworks.com/help/comm/ug/quantization.html
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
ql = 256; %quantization levels
max1 = max(y1);
min1 = min(y1);
interval = 2*max1/(ql-2); %value each level will increase by
partition = min1:interval:max1;
codebook = min1:interval:max1;


max2 = max(y3);
min2 = min(y3);
interval2 = 2*max2/(ql-2); %value each level will increase by
partition2 = min2:interval2:max2;
codebook2 = min2:interval2:max2;

%[index, quants] = quantiz(y1s, partition, codebook)
qy1 = zeros(1, length(y1s)); %quantized vectors
qy2 = zeros(1, length(y2s));
qy3 = zeros(1, length(y3s));
qyb1 = zeros(1, length(y1s)*8); %quantized binary vectors
qyb2 = zeros(1, length(y2s)*8); %quantized binary vectors
qyb3 = zeros(1, length(y3s)*8); %quantized binary vectors

for i = 1:length(t1)
  q1done = 0;
  q2done = 0;
  q3done = 0;
  for j = 1:length(partition)
    if((y1s(i) <= partition(j)) && q1done == 0)
      qy1(i) = codebook(j);
      q1done = 1;
      qyb1((8*i - 7):(8*i)) =  decimalToBinaryVector(j, 8, 'MSBFirst');
    end
    
    if((y2s(i) <= partition(j)) && q2done == 0)
      qy2(i) = codebook(j);
      q2done = 1;
      qyb2((8*i - 7):(8*i)) =  decimalToBinaryVector(j, 8, 'MSBFirst');
    end
    
    if((y3s(i) <= partition2(j)) && q3done == 0)
      qy3(i) = codebook2(j);
      q3done = 1;
      qyb3((8*i - 7):(8*i)) =  decimalToBinaryVector(j, 8, 'MSBFirst');
    end
  end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%plotting sampled signals
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure();
subplot(2, 2, 1);
stairs(t1, qy1);
title('y1 = sin(3000*pi*t) Sampled');

subplot(2, 2, 2);
stairs(t1, qy2);
title('y2 = cos(3000*pi*t) Sampled');

subplot(2, 1, 2);
stairs(t1, qy3);
title('y3 = sin(3000*pi*t) + cos(3000*pi*t) Sampled');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%compute hamming code
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = 0;
len = length(t1)*8;
pos = 1;
pow2 = 1;
qh1 = [];
qh2 = [];
qh3 = [];

while n < len %loop until all data bits are added
  if (pos) == pow2
    qh1=[qh1, -1];
    qh2=[qh2, -1];
    qh3=[qh3, -1];
    pow2 = bitsll(pow2, 1);
  else
    qh1=[qh1,qyb1(len - n)];
    qh2=[qh2,qyb2(len - n)];
    qh3=[qh3,qyb3(len - n)];
    n = n + 1;
  end
  pos = pos + 1;
end

XOR1 = 0;
XOR2 = 0;
XOR3 = 0;

for p = 1:length(qh1)
  if qh1(p) == 1
    XOR1 = bitxor(XOR1,p);
  end

  if qh2(p) == 1
    XOR2 = bitxor(XOR2,p);
  end
  
  if qh3(p) == 1
    XOR3 = bitxor(XOR3,p);
  end
end

numBits = length(qh1) - length(qyb1); %number of check bits
xor1 = decimalToBinaryVector(XOR1, numBits);
xor1 = fliplr(xor1);

xor2 = decimalToBinaryVector(XOR2, numBits);
xor2 = fliplr(xor2);

xor3 = decimalToBinaryVector(XOR3, numBits);
xor3 = fliplr(xor3);

n1 = 1;
n2 = 1;
n3 = 1;
%insert hamming code
for p= 1:length(qh1)
  if qh1(p) == -1
    qh1(p) = xor1(n1);
    n1 = n1 + 1;
  end
  
  if qh2(p) == -1
    qh2(p) = xor2(n2);
    n2 = n2 + 1;
  end
  
  if qh3(p) == -1
    qh3(p) = xor3(n3);
    n3 = n3 + 1;
  end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%perform TDM on all signals
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tdm = zeros(1, length(qh1)*3);

q1p = 1;
q2p = 1;
q3p = 1;

loopNum = ((length(tdm) - numBits*3)/8);
for n = 1:loopNum
  turn = mod(n,3);
  if(turn == 1)
    tdm(8*n - 7:8*n) = qh1(8*q1p - 7:  8*q1p);      
    q1p = q1p + 1;
  elseif(turn == 2)
    tdm(8*n - 7:8*n) = qh2(8*q2p - 7:  8*q2p);      
    q2p = q2p + 1;
  elseif(turn == 0)
    tdm(8*n - 7:8*n) = qh3(8*q3p - 7:  8*q3p);
    q3p = q3p + 1;
  end
end
tdm(265:271) = qh1(89:95);
tdm(272:278) = qh2(89:95);
tdm(279:285) = qh3(89:95);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%flip a random bit
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
bit = randi(length(tdm));
errorData = tdm;
errorData(bit) = bitxor(errorData(bit), 1);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%demultiplexing signal
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
recv1 = zeros(1, length(qh1));
recv2 = zeros(1, length(qh2));
recv3 = zeros(1, length(qh3));
q1p = 1;
q2p = 1;
q3p = 1;

for n = 1:loopNum
  turn = mod(n,3);
  if(turn == 1)
    recv1(8*q1p - 7:  8*q1p) = errorData(8*n - 7:8*n);      
    q1p = q1p + 1;
  elseif(turn == 2)
    recv2(8*q2p - 7:  8*q2p) = errorData(8*n - 7:8*n);      
    q2p = q2p + 1;
  elseif(turn == 0)
    recv3(8*q3p - 7:  8*q3p) = errorData(8*n - 7:8*n);
    q3p = q3p + 1;
  end  
end

recv1(89:95) = errorData(265:271);
recv2(89:95) = errorData(272:278);
recv3(89:95) = errorData(279:285);

%  a = isequal(recv1,qh1)
%  b = isequal(recv2,qh2)
%  c = isequal(recv3,qh3)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%error checking
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
pow2 = 1;
hammingCode1=[];
hammingCode2=[];
hammingCode3=[];
syndrome1=0;
syndrome2=0;
syndrome3=0;

%get hammingcode and start computing syndrome
for n=1:length(recv1)
  if (n) == pow2
    hammingCode1=[hammingCode1, recv1(n)];
    hammingCode2=[hammingCode2, recv2(n)];
    hammingCode3=[hammingCode3, recv3(n)];
    pow2 = bitsll(pow2, 1);
  else
    if recv1(n) == 1
      syndrome1 = bitxor(syndrome1, n);
    end
    if recv2(n) == 1
      syndrome2 = bitxor(syndrome2, n);
    end
    if recv3(n) == 1
      syndrome3 = bitxor(syndrome3, n);
    end
  end
end

hammingCode1 = fliplr(hammingCode1);
code1 = binaryVectorToDecimal(hammingCode1);

hammingCode2 = fliplr(hammingCode2);
code2 = binaryVectorToDecimal(hammingCode2);

hammingCode3 = fliplr(hammingCode3);
code3 = binaryVectorToDecimal(hammingCode3);

%final syndrome computation
syndrome1 = bitxor(syndrome1, code1);
syndrome2 = bitxor(syndrome2, code2);
syndrome3 = bitxor(syndrome3, code3);
fixedData1 = recv1;
fixedData2 = recv2;
fixedData3 = recv3;
%fix error
if(syndrome1 > 0)
  fixedData1(syndrome1) = bitxor(fixedData1(syndrome1), 1);
end

if(syndrome2 > 0)
  fixedData2(syndrome2) = bitxor(fixedData2(syndrome2), 1);
end

if(syndrome3 > 0)
  fixedData3(syndrome3) = bitxor(fixedData3(syndrome3), 1);
end

%  a = isequal(fixedData1, qh1)
%  b = isequal(fixedData2, qh2)
%  c = isequal(fixedData3, qh3)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%remove hamming code
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
n = 0;
len = length(fixedData1);
pos = 1;
pow2 = 1;
q1 = [];
q2 = [];
q3 = [];

while pos <= len %loop until all data bits are added
  if (pos) == pow2
    pow2 = bitsll(pow2, 1);
  else
    q1=[q1,fixedData1(pos)];
    q2=[q2,fixedData2(pos)];
    q3=[q3,fixedData3(pos)];
  end

  pos = pos + 1;
end

 q1 = fliplr(q1);
 q2 = fliplr(q2);
 q3 = fliplr(q3);
%  a = isequal(qyb1, q1)
%  b = isequal(qyb2, q2)
%  c = isequal(qyb3, q3)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%recover quantized samples
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
sig1 = zeros(1, length(qy1));
sig2 = zeros(1, length(qy2));
sig3 = zeros(1, length(qy3));

for n=1:((length(q1))/8)
  val1 =  binaryVectorToDecimal(q1(8*n - 7: 8*n));
  sig1(n) = codebook(val1);
  
  val2 =  binaryVectorToDecimal(q2(8*n - 7: 8*n));
  sig2(n) = codebook(val2);
  
  val3 =  binaryVectorToDecimal(q3(8*n - 7: 8*n));
  sig3(n) = codebook2(val3);
end

%  a = isequal(sig1, qy1)
%  b = isequal(sig2, qy2)
%  c = isequal(sig3, qy3)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%plotting recovered signals
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure();
subplot(2, 2, 1);
stairs(t1, sig1);
title('y1 = sin(3000*pi*t) Recovered');

subplot(2, 2, 2);
stairs(t1, sig2);
title('y2 = cos(3000*pi*t) Recovered');

subplot(2, 1, 2);
stairs(t1, sig3);
title('y3 = sin(3000*pi*t) + cos(3000*pi*t) Recovered');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
