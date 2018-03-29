%% SIMULATION
% Simulates numerically the generated field from a vacuum and a parametric amplifier driven by n pumps

close all
clear all
clc

h  = figure('Position', [100 100 1280 512]);
N = 2^18;
t = linspace(0, 1, N);
for pha = 0:pi/16:4*pi
    ain = complex(randn(1, N), randn(1, N))/sqrt(2);
    pin = dot(ain, ain);

    % 2 pumps
%     f = [-0.5e4 0.5e4];         % pump frequencies
%     p = [-0.5 pha];             % pump phases
%     A = [0.27 0.27];            % pump amplitudes
%     w = [-1.0e4 0 1.0e4];       % modes for analysis
%     k = 1.0;                    % kappa
%     W = 2^12;                   % window bandwidth

    % 3 pumps
    f = [-1.0e4 0.0e4 1.0e4];        % pump frequencies
    p = [0.00 pha 0.00];             % pump phases
    A = [0.23 0.23 0.23];            % pump amplitudes
    w = [-1.5e4 -0.5e4 0.5e4 1.5e4]; % modes for analysis
    k = 1.0;                         % kappa
    W = 2^12;                        % window bandwidth

    % 4 pumps
%     f = [-1.5e4 -0.5e4 0.5e4 1.5e4];   % pump frequencies
%     p = [pi/4 pha 0.00 pi/2];          % pump phases
%     A = [0.22 0.22 0.22 0.22];         % pump amplitudes
%     w = [-2.0e4 -1.0e4 0 1.0e4 2.0e4]; % modes for analysis
%     k = 1.0;                           % kappa
%     W = 2^12;                          % window bandwidth

    %%%% SOLVER %%%%%
    a(1) = 1;
    for x = 1:length(t)
        P = 0;
        for y = 1:length(f)
            P = P + A(y)*exp(j*p(y))*exp(j*2*pi*2*f(y)*t(x));
        end
        da(x) = conj(a(x))*P - k/2*a(x) - sqrt(k)*ain(x);
        a(x+1) = a(x) + da(x);
        aout(x+1) = (sqrt(k)*a(x) + ain(x) + sqrt(k)*a(x+1) + ain(x))/2;
    end

    %%%% Fourier transform the output %%%%
    F = fftshift(fft(aout));

    %%%% Plot spectrum %%%%
    figure(h);
    subplot(2, 4, 1);
    hold off;
    plot([-128:128], resample(abs(F).^2, 1, N/2^8)/N/2);
    hold on;
    for y = 1:length(f)
        plot([f(y)/(N/2^8) f(y)/(N/2^8)], [0 100], 'r');
    end
    for y = 1:length(w)
        plot([(w(y)-W)/(N/2^8) (w(y)-W)/(N/2^8)], [0 100], '--k');
        plot([(w(y)+W)/(N/2^8) (w(y)+W)/(N/2^8)], [0 100], '--k');
    end
    xlim([-50 50]);
    ylim([0 10])
    title('Power spectrum');

    %%%% Plot autocorrelation of complex spectrum as well as xcorr between its conjugate and flip %%%%
    figure(h);
    subplot(2, 4, 3);
    plot([-2^16:2^16], abs(xcorr(F, F, 2^16, 'coeff')));
    title('Autocorr.');
    ylim([0 1])
    subplot(2, 4, 4);
    plot([-2^16:2^16], abs(xcorr(conj(F), flip(F), 2^16, 'coeff')));
    title('Squeezing corr.');

    %%%% Extract relevant frequency windows and plot cov-matrix %%%%
    modedata_f = zeros(length(w), length(F(N/2+w(1)+1-W:N/2+w(1)+1+W))); % frequency domain data (analysis modes)
    modedata_t = zeros(length(w), length(F(N/2+w(1)+1-W:N/2+w(1)+1+W))); % time domain data (analysis modes)
    modedata_p = zeros(length(w), length(F(N/2+w(1)+1-W:N/2+w(1)+1+W))); % frequency domain data (surrounding pumps)
    Q = zeros(length(modedata_t(1, :)), length(w)*2);                    % individual time domain quadrature data (analysis modes)
    covm = 0;
    for y = 1:length(f)
        modedata_p(y, :) = F(N/2+f(y)+1-W:N/2+f(y)+1+W);
    end
    for y = 1:length(w)
        modedata_f(y, :) = F(N/2+w(y)+1-W:N/2+w(y)+1+W);
    end
    for y = 1:length(w)
        modedata_t(y, :) = ifft(ifftshift(modedata_f(y, :)));
    end
    for y = 1:length(w)
        Q(:, 2*y-1) = real(modedata_t(y, :));
        Q(:, 2*y)   = imag(modedata_t(y, :));
    end
    for x = 1:length(w)*2
        for y = 1:length(w)*2
            covm(x, y) = dot(Q(:, x), Q(:, y));
        end
    end
    figure(h);
    subplot(2, 4, 2);
    imagesc(covm/pin/2, [-max(max(abs(covm/pin/2))) max(max(abs(covm/pin/2)))]);
    caxis([-max(max(abs(covm/pin/2))) max(max(abs(covm/pin/2)))]);
    st = ['R_1';'I_1'];
    for y = 2:length(w)
        st = [st;'R_' num2str(y);'I_' num2str(y)];
    end
    set(gca, 'XTick', [1:length(w)*2]);
    set(gca, 'YTick', [1:length(w)*2]);
    set(gca, 'XTickLabel', st);
    set(gca, 'YTickLabel', st);
    axis([0.5 length(covm)+0.5 0.5 length(covm)+0.5])
    colorbar
    axis square
    title('Covariance matrix');

    %%%% Plot local quadrature histograms for each half pump %%%%
    figure(h);
    for y = 1:length(f)
        subplot(2, 4, 4+y); cla;
        S = ifft(ifftshift(modedata_p(y, :)));
        li = [-1:0.01:1]*100;
        HI  = hist3([real(S)' imag(S)'], {li li});
        surface(li, li, HI, 'edgecolor', 'none')
        axis square
        xlim([-100 100])
        ylim([-100 100])
    end

    drawnow;
end
