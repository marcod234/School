clc; clear all; close all;

user1 = [0,1,1,0,1,0,1,0,0,1];
user2 = [1,1,1,0,0,0,1,1,1,1];
user3 = [1,1,0,0,1,0,1,0,0,1];
user4 = [0,1,0,0,1,1,0,1,1,1];

subplot(2,2,1);
stairs(user1);
title('User 1')
ylim([-.5 1.5]);
xlim([1 11]);
ylabel('Amplitude')
xlabel('Bit')

subplot(2,2,2);
stairs(user2);
title('User 2')
ylim([-.5 1.5]);
xlim([1 11]);
ylabel('Amplitude')
xlabel('Bit')

subplot(2,2,3);
stairs(user3);
title('User 3')
ylim([-.5 1.5]);
xlim([1 11]);
ylabel('Amplitude')
xlabel('Bit')

subplot(2,2,4);
stairs(user4);
title('User 4')
ylim([-.5 1.5]);
xlim([1 11]);
ylabel('Amplitude')
xlabel('Bit')

dim = 8; %change for different dimension
walsh = zeros(dim); %preallocate 8x8 matrix
walsh(1,1) = 1; %1x1 walsh matrix

currDim = 1;

%generating the walsh matrix
while currDim < dim
    currDim = bitsll(currDim, 1); %go to the next dimension
    for i = 1:currDim %loop up to the current dimension amount of rows
      for j = 1:currDim %_________________________________________columns
          if ((i <= currDim/2) && (j > currDim/2)) %indices in the top right quadrant
              walsh(i,j) = walsh(i, j - currDim/2);
              
          elseif ((i > currDim/2) && (j <= currDim/2)) %indicies in the bottom left quadrant
              walsh(i,j) = walsh(i - currDim/2, j);
              
          elseif ((i > currDim/2) && (j > currDim/2)) %indicied in the bottom right quadrant
              walsh(i,j) = -walsh(i - currDim/2, j - currDim/2);
              
          else %%indicies in the top left quadrant
          end
      end
    end

end

%printing the matrix
walsh

u1Code = walsh(7, 1:dim); %get walsh code 6
u2Code = walsh(4, 1:dim); %walsh code 3
u3Code = walsh(2, 1:dim); %walsh code 1
u4Code = walsh(5, 1:dim); %walsh code 4

sendSize = length(u1Code)*length(user1);
u1Send = zeros(1,sendSize); %user spread signals
u2Send = zeros(1,sendSize);
u3Send = zeros(1,sendSize);
u4Send = zeros(1,sendSize);

%spread signals with corresponding code
for i = 1:length(user1)
  % copy up to length(Code) numbers at intervals
  % (1->CodeLength),(CodeLength+1 ->2*CodeLength)... etc 
  u1Send(((i*length(u1Code))-(length(u1Code)-1)):i*length(u1Code)) ...
    ...%if data is 1   copy normal code       else copy negative code 
    = (user1(i) == 1)*(u1Code) + (~(user1(i) == 1))*(-u1Code);

  u2Send(((i*length(u2Code))-(length(u2Code)-1)):i*length(u2Code)) ...
    = (user2(i) == 1)*(u2Code) + (~(user2(i) == 1))*(-u2Code);
  
  u3Send(((i*length(u3Code))-(length(u3Code)-1)):i*length(u3Code)) ...
    = (user3(i) == 1)*(u3Code) + (~(user3(i) == 1))*(-u3Code);
  
  u4Send(((i*length(u4Code))-(length(u4Code)-1)):i*length(u4Code)) ...
    = (user4(i) == 1)*(u4Code) + (~(user4(i) == 1))*(-u4Code);
end

combined = u1Send + u2Send + u3Send + u4Send; %combine the signals

figure;
stairs(combined);
title('Combined Signals')
ylim([-5 5]);
xlim([1 length(u1Send)+1]);
ylabel('Amplitude')
xlabel('Bit')

%%reciever side - despreading

u1Dec = zeros(1,length(user1)); %decoded signals
u2Dec = zeros(1,length(user2));
u3Dec = zeros(1,length(user3));
u4Dec = zeros(1,length(user4));

for i = 1:length(user1)
  bit = combined(((i*length(u1Code))-(length(u1Code)-1)):i*length(u1Code));
  u1Dec(i) = sum(bit .* u1Code); %mulitply the signal by each spreading
  u2Dec(i) = sum(bit .* u2Code); %code and add up the values to get the
  u3Dec(i) = sum(bit .* u3Code); %resulting bit
  u4Dec(i) = sum(bit .* u4Code);
  
  %scale signal back to 1's and 0's
  u1Dec(i) = (u1Dec(i) > 0)*1 + (u1Dec(i) < 0)*0;
  u2Dec(i) = (u2Dec(i) > 0)*1 + (u2Dec(i) < 0)*0;
  u3Dec(i) = (u3Dec(i) > 0)*1 + (u3Dec(i) < 0)*0;
  u4Dec(i) = (u4Dec(i) > 0)*1 + (u4Dec(i) < 0)*0;
end

%plot figures
figure;
subplot(2,2,1);
stairs(u1Dec);
title('User 1 Decoded')
ylim([-.5 1.5]);
xlim([1 11]);
ylabel('Amplitude')
xlabel('Bit')

subplot(2,2,2);
stairs(u2Dec);
title('User 2 Decoded')
ylim([-.5 1.5]);
xlim([1 11]);
ylabel('Amplitude')
xlabel('Bit')

subplot(2,2,3);
stairs(u3Dec);
title('User 3 Decoded')
ylim([-.5 1.5]);
xlim([1 11]);
ylabel('Amplitude')
xlabel('Bit')

subplot(2,2,4);
stairs(u4Dec);
title('User 4 Decoded')
ylim([-.5 1.5]);
xlim([1 11]);
ylabel('Amplitude')
xlabel('Bit')

u1Err = 0;
u2Err = 0;
u3Err = 0;
u4Err = 0;

%%compute bit error
for i = 1:length(user1)
   %if user(i) and u1Dec(i) are not equal then add 1 to total error count
   u1Err = (~(user1(i) == u1Dec(i))) + u1Err;
   u2Err = (~(user2(i) == u2Dec(i))) + u2Err;
   u3Err = (~(user3(i) == u3Dec(i))) + u3Err;
   u4Err = (~(user4(i) == u4Dec(i))) + u4Err;
end

u1BER = u1Err/length(user1)
u2BER = u2Err/length(user2)
u3BER = u3Err/length(user3)
u4BER = u4Err/length(user4)

