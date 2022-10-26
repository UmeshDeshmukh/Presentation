close all;
clear;
load ecg_clean.txt
load ecg_noisy.txt

clean_sig = ecg_clean;
noisy_sig = ecg_noisy;


Fs = 200;
Ts = 1/Fs;
f  = 50;
w  = 2*pi*f; 
n  = 0:Ts:(5-Ts);
A  = 1.0;
intf_sig =  A*sin(w*n);

signal1 = ecg_noisy + intf_sig';

figure(1);
subplot(3,2,1);
plot(clean_sig);
title('Clean Signal');
subplot(3,2,3);
plot(noisy_sig);
title('Noisy Signal');
subplot(3,2,5);
plot(signal1);
title('Noisy signal with mains interference');

subplot(3,2,2);
stem(abs(fft(clean_sig)));
title('Spectrum of Clean Signal');
subplot(3,2,4);
stem(abs(fft(noisy_sig)));
title('Spectrum of Noisy Signal');
subplot(3,2,6);
stem(abs(fft(signal1)));
title('Spectrum of Noisy signal with mains(50Hz) interference');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% LEAST SQUARE LOWPASS FILTER with SPECIFIED NULL
% filter length
N = 31;
M = (N-1)/2;
% set band-edges and stop-band weighting
wp = 0.10*pi;
ws = 0.20*pi;
K = 8;
% set null
w1 = 0.5*pi;
% normalize band-edges for convenience
fp = wp/pi;
fs = ws/pi;
% construct q(k)
q = [fp+K*(1-fs), fp*sinc(fp*[1:2*M])-K*fs*sinc(fs*[1:2*M])];
% construct Q1, Q2, Q
Q1 = toeplitz(q([0:M]+1));
Q2 = hankel(q([0:M]+1),q([M:2*M]+1));
Q= (Q1 + Q2)/2;
% construct b
b= fp*sinc(fp*[0:M]');
% construct G and d
G= cos([0:M]*w1);
d= [0];
% solve linear system for Lagrange multipliers
c = Q\b;
mu = (G*inv(Q)*G')\(G*c-d);
% solve linear system for filter coeffs
a= c-Q\(G'*mu);
% form impulse response h(n)
h= [a(M+1:-1:2); 2*a(1); a(2:M+1)]/2;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

b_1 = firls(10,[0,0.10,0.2,1],[1 1 0 0],[10 90]);
b_2 = firls(30,[0,0.10,0.2,1],[1 1 0 0],[20 80]);
b_3 = firls(20,[0,0.10,0.2,1],[1 1 0 0],[10 90]);

fir_out1 = filter(b_1,[1],signal1);
fir_out2 = filter(b_2,[1],signal1);
fir_out3 = filter(b_3,[1],signal1);
fir_out4 = filter(h,[1],signal1);

RMSE1 = sqrt(mean((clean_sig-fir_out1).^2));
RMSE2 = sqrt(mean((clean_sig-fir_out2).^2));
RMSE3 = sqrt(mean((clean_sig-fir_out3).^2));
RMSE4 = sqrt(mean((clean_sig-fir_out4).^2));
display(RMSE1);
display(RMSE2);
display(RMSE3);
display(RMSE4);

figure(5);

sgtitle('Filtered output of Signal with noise and mains interference');
subplot(4,2,1);
plot(fir_out1);
title('Filter (order 10) output');
subplot(4,2,3);
plot(fir_out2);
title('Filter (order 30) output');
subplot(4,2,5);
plot(fir_out3);
title('Filter (order 20) output');
subplot(4,2,7);
plot(fir_out4);
title('Filter (order 30 with 50 Hz NULL) output');

out1_f = abs(fft(fir_out1));
subplot(4,2,2);
semilogy(out1_f);
title('Filtered (order 10) output spectrum');
out2_f = abs(fft(fir_out2));
subplot(4,2,4);
semilogy(out2_f);
title('Filtered (order 30) output spectrum');
out3_f = abs(fft(fir_out3));
subplot(4,2,6);
semilogy(abs(fft(fir_out3)));
title('Filtered (order 20) output spectrum');
out4_f = abs(fft(fir_out4));
subplot(4,2,8);
semilogy(abs(fft(fir_out4)));
title('Filtered (order 30 with 50 Hz NULL) output spectrum');

figure(6)
subplot(2,2,1);
zplane(b_1,[1]);
title('Filter (order 10) PZ plot');
subplot(2,2,2);
zplane(b_2,[1]);
title('Filter (order 30) PZ plot');
subplot(2,2,3);
zplane(b_3,[1]);
title('Filter (order 20) PZ plot');
subplot(2,2,4);
zplane(h,[1]);
title('Filter(order 30 with 50 Hz NUL) PZ plot');

figure(7)
freqz(h,[1]);


