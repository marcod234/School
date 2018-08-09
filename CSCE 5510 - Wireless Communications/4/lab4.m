clc; clear all; close all;

%you can change data bits for different result
data = [0,1,1,1,0,0,1,0];
secData= [];
len = length(data);
n = 0;
pos = 1;
pow2 = 1;

while n < len %loop until all data bits are added
  if (pos) == pow2
    secData=[secData, -1];
    pow2 = bitsll(pow2, 1);
  else
    secData=[secData,data(len - n)];
    n = n + 1;
  end
  pos = pos + 1;
end

%compute hamming code
XOR = 0;
for p = 1:length(secData)
  if secData(p) == 1
    XOR = bitxor(XOR,p); 
  end
end


xor = decimalToBinaryVector(XOR);

xor = fliplr(xor);


n = 1;
%insert hamming code
for p= 1:length(secData)
  if secData(p) == -1
    secData(p) = xor(n);
    n = n + 1;
  end
end

%flip a random bit
bit = randi(length(secData));
errorData = secData;
errorData(bit) = bitxor(errorData(bit), 1);


pow2 = 1;
hammingCode=[];
syndrome=0;
%get hammingcode and start computing syndrome
for n=1:length(errorData)
  if (n) == pow2
    hammingCode=[hammingCode, errorData(n)];
    pow2 = bitsll(pow2, 1);
  else
    if errorData(n) == 1
      syndrome = bitxor(syndrome, n);
    end
  end
end

hammingCode = fliplr(hammingCode);
code = binaryVectorToDecimal(hammingCode);
%final syndrome computation
syndrome = bitxor(syndrome, code)
%fix error
fixedData = errorData;
fixedData(bit) = bitxor(fixedData(syndrome), 1);

%plotting code
subplot(2,2,1);
stairs(data);
title('Data')
ylim([-.5 1.5]);
xlim([1 length(data)+1]);
ylabel('Amplitude')
xlabel('Bit')

subplot(2,2,2);
stairs(fliplr(secData));
title('Transmitted Data')
ylim([-.5 1.5]);
xlim([1 length(secData)+1]);
ylabel('Amplitude')
xlabel('Bit')

subplot(2,2,3);
stairs(fliplr(errorData));
title('Transmitted Data with Error')
ylim([-.5 1.5]);
xlim([1 length(errorData)+1]);
ylabel('Amplitude')
xlabel('Bit')

subplot(2,2,4);
stairs(fliplr(fixedData));
title('Transmitted Data Fixed')
ylim([-.5 1.5]);
xlim([1 length(errorData)+1]);
ylabel('Amplitude')
xlabel('Bit')