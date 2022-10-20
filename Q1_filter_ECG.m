close all
clear

load ecg_clean.txt
load ecg_noisy.txt

clean_sig = ecg_clean;
noisy_sig = ecg_noisy;
%%
X = fft(noisy_sig);
figure(1)
subplot(3,1,1)
stem(abs(fft(clean_sig)))
title('spectrum of clean signal')
subplot(3,1,2)
stem(abs(fft(noisy_sig)))
title('spectrum of noisy signal')
subplot(3,1,3)
stem(abs(fft(noisy_sig)-fft(clean_sig)))
title('spectrum of noise')
%%  FIR low pass filter 

% b1 = firls(20,[0,0.20,0.4,1],[1 1 0 0],[1 100]);
b_2 = firls(30,[0,0.10,0.2,1],[1 1 0 0],[50 50]);
% b3 = firls(30,[0,0.046,0.2,1],[1 1 0 0],[100 1]);
fir_out = filter(b_2,[1],noisy_sig);

figure(7)
freqz(b_2,[1])
title('FIR filter frequency response')
l  = length(clean_sig)
l2 = length(fir_out)

absS1 = max(abs(clean_sig))
% absO = max(abs(f1))
% plot(abs(clean_sig(1:l-30) - fir_out(31:l2)));

%% IIR low pass filter

% [b2, a2] = cheby2(10,40,0.25,'low');
[b2, a2] = cheby2(10,40,0.25,'low');
cheby2_out = filter(b2,a2,noisy_sig);

figure(4)
subplot(3,1,1)
plot(clean_sig)
title('Clean signal')
subplot(3,1,2)
plot(fir_out)
title('FIR filter output')
subplot(3,1,3)
plot(cheby2_out)
title('IIR filter output')

% subplot(3,1,3)
% plot(butter_out)

%%
figure(6)
freqz(b2,a2)
title('IIR filter frequency response')
figure(5)
subplot(2,1,1)
zplane(b2,a2)
title('IIR filter pole zero plot')
subplot(2,1,2)
zplane(b_2,[1])
title('FIR filter pole zero plot')

