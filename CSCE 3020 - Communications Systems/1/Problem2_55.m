x = randn(1,256);
X = fft(x);
Xlpf = X;

for k=9:248
    Xlpf(1,k)=0;
end

xlpf = real(ifft(Xlpf));
t = 0:1:255;

plot(t,x,'color','r'); hold on;
plot(t,xlpf,'color','b');