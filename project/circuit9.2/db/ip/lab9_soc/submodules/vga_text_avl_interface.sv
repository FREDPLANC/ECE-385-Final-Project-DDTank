/************************************************************************
Avalon-MM Interface VGA Text mode display

Modified for DE2-115 board

Register Map:
0x000-0x0257 : VRAM, 80x30 (2400 byte, 600 word) raster order (first column then row)
0x258        : control register

VRAM Format:
X->
[ 31  30-24][ 23  22-16][ 15  14-8 ][ 7    6-0 ]
[IV3][CODE3][IV2][CODE2][IV1][CODE1][IV0][CODE0]

IVn = Draw inverse glyph
CODEn = Glyph code from IBM codepage 437

Control Register Format:
[[31-25][24-21][20-17][16-13][ 12-9][ 8-5 ][ 4-1 ][   0    ] 
[[RSVD ][FGD_R][FGD_G][FGD_B][BKG_R][BKG_G][BKG_B][RESERVED]

VSYNC signal = bit which flips on every Vsync (time for new frame), used to synchronize software
BKG_R/G/B = Background color, flipped with foreground when IVn bit is set
FGD_R/G/B = Foreground color, flipped with background when Inv bit is set

************************************************************************/
`define NUM_REGS 601 //80*30 characters / 4 characters per register
`define CTRL_REG 600 //index of control register

module vga_text_avl_interface (
	// Avalon Clock Input, note this clock is also used for VGA, so this must be 50Mhz
	// We can put a clock divider here in the future to make this IP more generalizable
	input logic CLK,
	
	// Avalon Reset Input
	input logic RESET,
	
	// Avalon-MM Slave Signals
	input  logic AVL_READ,					// Avalon-MM Read
	input  logic AVL_WRITE,					// Avalon-MM Write
	input  logic AVL_CS,					// Avalon-MM Chip Select
	input  logic [3:0] AVL_BYTE_EN,			// Avalon-MM Byte Enable
	input  logic [9:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,		// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,		// Avalon-MM Read Data
	
	// Exported Conduit (mapped to VGA port - make sure you export in Platform Designer)
	output logic [3:0]  red, green, blue,	// VGA color channels (mapped to output pins in top-level)
	output logic hs, vs,					// VGA HS/VS
	output logic sync, blank, pixel_clk		// Required by DE2-115 video encoder
);

logic [31:0] LOCAL_REG       [`NUM_REGS]; // Registers
//put other local variables here
logic [9:0] DrawX,DrawY;
logic [10:0] sprite_addr;
logic [7:0] sprite_data;
logic [31:0] read_buf,reg_value,color_value;
logic  inverse, RESET_REQ;
int shape_size_x, shape_size_y, shape_x, shape_y, value_index,reg_addr;
logic [6:0] data;
logic [2:0] bit_num;
logic [31:0] temp_read, temp_write;



//Declare submodules..e.g. VGA controller, ROMS, etc
vga_controller vga(.Clk(CLK),
					.Reset(RESET),
					.*);
font_rom mapping(.addr(sprite_addr), .data(sprite_data));

lab9_soc_ocm ocm1( 					// inputs:
                                    .address(AVL_ADDR),
                                    .byteenable(AVL_BYTE_EN),
                                    .chipselect(AVL_CS),
                                    .clk(CLK),
                                    .clken(1'b1),
                                    .freeze(1'b0),
                                    .reset(RESET),
                                    .reset_req(RESET_REQ),
                                    .write(AVL_WRITE),
                                    .writedata(temp_write),

                                   // outputs:
                                    .readdata(temp_read)
									);

lab9_soc_ocm ocm2( 					// inputs:
                                    .address(reg_addr),
                                    .byteenable(4'b1111),
                                    .chipselect(AVL_CS),
                                    .clk(CLK),
                                    .clken(1'b1),
                                    .freeze(1'b0),
                                    .reset(RESET),
                                    .reset_req(RESET_REQ),
                                    .write(1'b0),
                                    .writedata(32'bX),

                                   // outputs:
                                    .readdata(reg_value)
									);

//*****************************************************************

always_ff @(posedge CLK) 
begin
	if(RESET)
	begin
		RESET_REQ <= 1'b1;
	end
	else
	begin
		RESET_REQ <= 1'b0;
	end
	if (AVL_CS & AVL_READ)
	begin
		AVL_READDATA <= temp_read;
	end
	else
	begin
		AVL_READDATA <= 32'b0;
	end
	if (AVL_CS & AVL_WRITE)
 		begin
 			unique case (AVL_BYTE_EN)
 			4'b1111: temp_write <= AVL_WRITEDATA;
 			4'b1100: temp_write[31:16]  <= AVL_WRITEDATA[31:16];
 			4'b0011: temp_write[15:0]  <= AVL_WRITEDATA[15:0];	
 			4'b1000: temp_write[31:24]  <= AVL_WRITEDATA[31:24];
 			4'b0100: temp_write[23:16]  <= AVL_WRITEDATA[23:16];
 			4'b0010: temp_write[15:8]  <= AVL_WRITEDATA[15:8];
 			4'b0001: temp_write[7:0]  <= AVL_WRITEDATA[7:0];
 			default:;
 			endcase
 		end

end
//***********************************************************************



//handle drawing (may either be combinational or sequential - or both).
assign color_value = LOCAL_REG[600];

always_comb
begin

	reg_addr = (DrawY[9:4]) * 20 + DrawX[9:5];
	value_index = (DrawX[9:3]) %4;

	case (value_index)
		3: 
		begin
			inverse = reg_value[31];
			data = reg_value[30:24];
		end
		2: 
		begin
			inverse = reg_value[23];
			data = reg_value[22:16];
		end
		1: 
		begin
			inverse = reg_value[15];
			data = reg_value[14:8];
		end
		0: 
		begin
			inverse = reg_value[7];
			data = reg_value[6:0];
		end
		default:
		begin
			inverse = 1'b0;
			data = 7'b0;
		end
	endcase
	sprite_addr =  {data, DrawY[3:0]}; 
	bit_num = 3'b111 - DrawX[2:0];

end


always_ff @(posedge pixel_clk)
		begin
		if(~blank)
		begin
			red <= 4'b0;
			green <= 4'b0;
			blue <= 4'b0;
		end
		else
		begin
		if(sprite_data[bit_num])
		begin
			if(inverse)
			begin
				red <= color_value[12:9];
				green <= color_value[8:5];
				blue <= color_value[4:1];
			end
			else begin
				red <= color_value[24:21];
				green <= color_value[20:17];
				blue <= color_value[16:13];
			end
		end
		else begin
			if(inverse)
			begin
				red <= color_value[24:21];
				green <= color_value[20:17];
				blue <= color_value[16:13];
			end
			else begin
				red <= color_value[12:9];
				green <= color_value[8:5];
				blue <= color_value[4:1];
			end
		end

		end

end



endmodule

