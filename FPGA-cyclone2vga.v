module test(clock, led, out0i, out1i, out0q, out1q);

input clock;
output [2:0] led;
output out0i;
output out1i;
output out0q;
output out1q;

reg vga_HS, vga_VS;
reg [9:0] CounterX;
reg [8:0] CounterY;
reg [9:0] temporary;
reg [9:0] cntx;
reg [9:0] cnty;
reg pixvalue;

reg [7:0] fnt[6399:0];
initial $readmemh("FPGA-fnt.hex", fnt);

always @(posedge pll_clock)
begin	
	CounterX <= CounterX + 1;
	
	if(CounterX==800)
	begin
		CounterX <= 0;
		CounterY <= CounterY + 1;
	end

	if(CounterY==449)
	begin
		CounterY <= 0;
		temporary <= temporary + 1;
	end
	
	vga_HS <= (CounterX>640+16 & CounterX<800-48);
	vga_VS <= (CounterY>400+12 & CounterY<449-35);
	
	cntx <= CounterX + temporary;
	cnty <= CounterY + (temporary >> 2);
	
	if(CounterY>8)
		pixvalue <= ((CounterX<640 & CounterY<400) & (CounterX[2] & CounterY[2])) | ((CounterX>128 & CounterX<512 & CounterY>64 & CounterY<300) & (cntx[4] & cnty[4]));
	else
		if(CounterX<640) pixvalue <= (fnt[CounterX+CounterY*760]>1);
end

assign out0i = ~vga_HS;
assign out1i = ~vga_VS;
assign out0q = pixvalue;

// assign led = cnt2[23:21];

altpll0 altpll0 (
	 .inclk0(clock),
	 .c0(pll_clock)
 );

endmodule