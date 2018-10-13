module VGA(input clock, output [2:0] led, output hsync, output vsync, output g);
	reg [31:0] cnt;
	reg [31:0] cnt25;
	reg  [7:0] dataa;
	reg  [7:0] addra;
	reg wea;
	wire [7:0] q;
	
	/* GPU related */
	reg clk25, gval;
	reg [9:0] x, y;
	reg fnt[16383:0];       /* 8x8 character set (2 kB) */

	/* CPU related */
	reg  [7:0] a, b, c, d;
	reg [15:0] opcode, ip;
	reg [15:0] data1, data2;
	reg [15:0] r[3:0];      /* general purpose registers */

	integer j;
	initial begin
		$readmemh("d:/CGA.hex", fnt);
		ip = 0;
		opcode = 0;
		wea = 0;
	end

always @(posedge clock) begin
	cnt <= cnt + 1;
	clk25 <= ~clk25;
	
	if(clk25==1) begin
		wea <= 0;
		if(x < 799)
			x <= x + 1;
		else begin
			x <= 0;
			if(y < 449)
				y <= y + 1;
			else
			y <= 0;
		end
		
		if((x < 640) && (y < 400)) begin
			addra <= (x>>4);
			gval <= (q>>(x%8))&1;		
		end else begin
			gval <= 0;
		end
	end
	
	if(clk25==0) begin
		if(cnt25<100000) begin
			cnt25 <= cnt25 + 1;
			wea <= 1;
			addra <= cnt25&255;
			dataa <= cnt25&255;
		end
	end
end

/* vga related */
assign hsync = x < (640 + 16) || x >= (640 + 16 + 96);
assign vsync = y < (400 + 12) || y >= (400 + 12 + 2);
assign g = gval;

assign led = cnt[22:20];

ram2 ram2(addra, address_b, clock,	dataa, data_b, wea, wren_b, q, q_b);

endmodule