module test(input clock, output [2:0] led, output VGA_HS, VGA_VS, VGA_B);

wire cpu_clock;
wire pixel_clock;


/**** VGA ****/

/* 640x400 70Hz VESA according to http://tinyvga.com/vga-timing/640x400@70Hz */
parameter H   = 640; // width of visible area
parameter HFP = 16;  // unused time before hsync
parameter HS  = 96;  // width of hsync
parameter HBP = 48;  // unused time after hsync

parameter V   = 400; // height of visible area
parameter VFP = 12;  // unused time before vsync
parameter VS  = 2;   // width of vsync
parameter VBP = 35;  // unused time after vsync

reg[9:0] h_cnt;      // horizontal pixel counter
reg[9:0] v_cnt;      // vertical pixel counter

reg hs, vs, pixel;
reg vmem [320*200-1:0];
reg [15:0] video_counter;

/* horizontal pixel counter */
always@(posedge pclk) begin
	if(h_cnt==H+HFP+HS+HBP-1) h_cnt <= 0;
	else                      h_cnt <= h_cnt + 1;
	if(h_cnt == H+HFP)    hs <= 0;
	if(h_cnt == H+HFP+HS) hs <= 1;
end

/* vertical pixel counter */
always@(posedge pclk) begin
	if(h_cnt == H+HFP) begin
		if(v_cnt==VS+VBP+V+VFP-1) v_cnt <= 0; 
		else                      v_cnt <= v_cnt + 1;
 		if(v_cnt == V+VFP)    vs <= 1;
		if(v_cnt == V+VFP+VS) vs <= 0;
	end
end

/* RAMDAC */
always@(posedge pclk) begin
	if((v_cnt < V) && (h_cnt < H)) begin
		if(h_cnt[0] == 1)
			video_counter <= video_counter + 1;
		pixel <= vmem[video_counter];
	end else begin
		if(h_cnt == H+HFP) begin
			if(v_cnt == V+VFP)
				video_counter <= 0;
			else if((v_cnt < V) && (v_cnt[0] != 1))
				video_counter <= video_counter - 320;
		end			
		pixel <= 0;
	end
end

assign VGA_HS = hs;
assign VGA_VS = vs;
assign VGA_B = pixel;

/**** VGA ****/


reg [31:0] mem[255:0];
reg [31:0] D;
reg [31:0] pc;
reg [31:0] cnt;
reg [31:0] r[31:0], data;
reg [7:0] o, a, b, c;
reg pclk, cclk;

initial $readmemh("d:/clean/ram.hex", mem);

initial begin
	pc = 0;
	o = 0;
	a = 0;
	b = 0;
	c = 0;
end

always @(posedge clock)
begin
	cnt <= cnt + 1;
	pclk <= ~pclk;  /* 25 MHz */
	cclk <= ~cclk;
	// cclk <= cnt[2];
end

always @(posedge cpu_clock)
begin
	D <= mem[pc];
	o <= D[31:24];
	a <= D[23:16];
	b <= D[15:8];
	c <= D[7:0];
end

always @(negedge cpu_clock)
begin
	data <= mem[pc+1];
	case(o)
		 1:begin /* JMP */
				pc <= data;
			end
		 6:begin /* JL */
				if(r[a]<r[b])
					pc <= data;
				else
					pc <= pc + 1;
			end
		 8:begin /* MOV */
				r[a] <= data;
				pc <= pc + 2;
			end
		11:begin /* STO */
				vmem[r[a]] <= r[b];
				pc <= pc + 1;
			end
		12:begin /* SHL */
				r[a] <= r[b] << r[c];
				pc <= pc + 1;
			end
		13:begin /* SHR */
				r[a] <= r[b] >> r[c];
				pc <= pc + 1;
			end
		17:begin /* XOR */
				r[a] <= r[b] ^ r[c];
				pc <= pc + 1;
			end
		18:begin /* ADD */
				r[a] <= r[b] + r[c];
				pc <= pc + 1;
			end
	endcase
end

assign pixel_clock = pclk;
assign cpu_clock = cclk;

/* LED related */
assign led = cnt[23:21];

endmodule