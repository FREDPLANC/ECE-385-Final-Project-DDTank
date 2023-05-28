//Verilog HDL Single-Clock True Dual-Port Synchronous RAM
module ocm(
	input logic [3:0] write_in,		//data to be written
	input logic [18:0] read_addr, write_addr,				//address to be written to 2^10 = 1024
	input int clk,
	output logic [3:0] read_out	//data to be readout
);

parameter DATA_WIDTH = 4;
parameter ADDR_WIDTH = 19;

reg [DATA_WIDTH-1:0] ram[ADDR_WIDTH-1:0];

always @ (posedge clk)
begin //AVL port
		ram[write_addr +97 ] <= write_in;

		
end

assign read_out = ram[read_addr];
//
//always @ (posedge clk) 
//begin
//	if (vga_we)
//		ram[vga_addr] <= vga_writein;
//	vga_readout <= ram[vga_addr];
//end

endmodule
