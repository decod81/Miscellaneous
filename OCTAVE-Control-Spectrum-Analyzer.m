pkg load instrument-control;
s0 = serial("/dev/ttyUSB0", 115200);
srl_write(s0, "CF 2000MZ;\n"); % Center Frequency
srl_write(s0, "SP 100MZ;\n"); % Span
srl_write(s0, "RB 1MZ;\n"); % Resolution Bandwidth (1k, 3k, 10k...3M)
srl_write(s0, "VB 300Z;\n"); % Video Bandwidth Ratio (30, 100...3M)
srl_write(s0, "SNGLS;\n"); % Single Sweep
srl_write(s0, "TS;\n"); % Take Sweep
srl_write(s0, "TA;\n");
srl_write(s0, "++read eoi\n");
data = srl_read(s0, 3208);
a = strread(char(data));
fclose(s0);
