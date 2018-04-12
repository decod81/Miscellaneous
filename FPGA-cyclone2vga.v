module test(clock, led, HS, VS, VAL, OUTA, OUTB, OUTC, OUTD);

input clock;
output [2:0] led;
output HS, VS, VAL;
output OUTA, OUTB, OUTC, OUTD;

/* VGA related */
reg vga_HS, vga_VS;
reg [9:0] CounterX;
reg [8:0] CounterY;
reg [9:0] temporary;
reg [9:0] cntx;
reg [9:0] cnty;
reg pixvalue;
reg [7:0] fnt[6399:0];
initial $readmemh("d:/clean/FPGA/Cyclone2/fnt.hex", fnt);

/* DAC related */
reg [31:0] cnt;

/* PLL2 related */
reg [23:0] cnt2;

/* VGA and PLL1 related */
always @(posedge pll_clock)
begin
	/* DAC related */
	cnt <= cnt + 1;
	
	/* VGA related */
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

/* PLL2: currently set to 105 MHz, max 500 MHz */
reg outa;
reg [14:0] val;
reg [7:0] snd0;
reg [31:0] snd;

always @(posedge pll2_clock)
begin
	cnt2 <= cnt2 + 1;
	
	/* a kind of radio transmitter */
	val <= val + 1;
	snd = cnt >> 16;	
	snd0 = snd&snd%255-(snd*3&snd>>13&snd>>6);
	// snd0 = (snd*5&snd>>7)|(snd*3&snd>>10);
	if((val[0]==0) & (snd0>1))
		outa = 0;
	else
		outa = 1;
end

/* VGA related */
assign HS = ~vga_HS;
assign VS = ~vga_VS;
assign VAL = pixvalue;

/* DAC related */
assign OUTC = cnt[5];
assign OUTA = cnt[6];
assign OUTB = cnt[7];

/* PLL2 related */
// assign OUTD = cnt2[10];
assign OUTD = outa; // radio

/* LED related */
assign led = cnt[23:21];

altpll0 altpll0 (
	 .inclk0(clock),
	 .c0(pll_clock),
	 .c1(pll2_clock)
 );

 /* LVDS related
 module test(clock, txout, txoutclock);
	input clock;
	output txout;
	output txoutclock;
	reg [7:0] txin;

	always @ (posedge clock)
	begin
		txin <= txin + 8;
	end
	
	lvds lvds(.tx_in(txin), .tx_inclock(clock), .tx_out(txout), .tx_outclock(txoutclock));
endmodule
 */
 
endmodule