module test(clock, led, out0i, out1i, out0q, out1q);

input clock;
output [2:0] led;
output out0i;
output out1i;
output out0q;
output out1q;

reg a, b;
reg [12:0] cnt1;
reg [23:0] cnt2;

reg [7:0] MEMi[8191:0];
reg [7:0] MEMq[8191:0];

integer n;

initial begin
	for(n=0; n<1365; n=n+1)
	begin
		MEMi[n] = 0;
		MEMq[n] = 1;
	end
	
	for(n=1365; n<2730; n=n+1)
	begin
		MEMi[n] = 1;
		MEMq[n] = 2;
	end

	for(n=2730; n<4096; n=n+1)
	begin
		MEMi[n] = 2;
		MEMq[n] = 3;
	end

	for(n=4096; n<5461; n=n+1)
	begin
		MEMi[n] = 3;
		MEMq[n] = 2;
	end

	for(n=5461; n<6826; n=n+1)
	begin
		MEMi[n] = 2;
		MEMq[n] = 1;
	end

	for(n=6826; n<8192; n=n+1)
	begin
		MEMi[n] = 1;
		MEMq[n] = 0;
	end
end

always @(posedge pll_clock) // 500 MHz
begin
	 cnt1 <= cnt1 + 1;
end

always @(posedge clock) // 50 MHz
begin
	 cnt2 <= cnt2 + 1;
end

assign out0i = MEMi[cnt1][0];
assign out1i = MEMi[cnt1][1];
assign out0q = MEMq[cnt1][0];
assign out1q = MEMq[cnt1][1];
assign led = cnt2[23:21];

altpll0 altpll0 (
	 .inclk0(clock),
	 .c0(pll_clock)
 );

endmodule