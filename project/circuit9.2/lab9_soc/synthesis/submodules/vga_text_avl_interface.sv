
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
`define palette_num 8 //index of control register

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
	input  logic [8:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,		// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,		// Avalon-MM Read Data
	
	// Exported Conduit (mapped to VGA port - make sure you export in Platform Designer)
	output logic [3:0]  red, green, blue,	// VGA color channels (mapped to output pins in top-level)
	output logic hs, vs,					// VGA HS/VS
	output logic sync, blank, pixel_clk		// Required by DE2-115 video encoder
);

//logic [31:0] LOCAL_REG       [`NUM_REGS]; // Registers
logic [31:0] Palette_table [`palette_num];
//put other local variables here
logic [9:0] DrawX,DrawY;
logic [10:0] sprite_addr;
logic [7:0] sprite_data;
logic [31:0] read_buf,reg_value,color_value;
logic  inverse, RESET_REQ,write_req, CS_REQ,EN_REQ,ADDR;
int shape_size_x, shape_size_y, shape_x, shape_y, value_index,reg_addr;
//logic [6:0] data;
//logic [2:0] bit_num;
//logic [31:0] temp_read, temp_write;
//logic [3:0] F_index, B_index;



//project variable******************************************************************
logic [18:0] write_addr;
int read_addr;
//logic [3:0] byte_en;
logic [3:0] temp_write_2, read_value;
logic [31:0] table_val        [120];
logic [31:0] table_ele        [120];
logic [31:0] table_rom			[120];
logic [3:0] color_index;
logic [3:0] color_index_1;
logic [3:0] color_index_2;
logic [3:0] color_index_3;
logic [3:0] color_index_4;
logic [3:0] color_index_5;
logic [3:0] color_index_6;
logic [3:0] table_color       [120];
logic [5:0]table_exist       [120];
//logic right                   [3];




//====================================================================================================


//Declare submodules..e.g. VGA controller, ROMS, etc
vga_controller vga(.Clk(CLK),
					.Reset(RESET),
					.*);


//OCM OCM(
//											.address_a(AVL_ADDR[10:0]),
//											.address_b(reg_addr[10:0]),
//											.byteena_a(AVL_BYTE_EN),
//											.clock(CLK),
//											.data_a(temp_write),
//											.data_b(32'bX),
//											.rden_a(AVL_READ),
//											.rden_b(1'b1),
//											.wren_a(AVL_WRITE & (~AVL_ADDR[11])),
//											.wren_b(1'b0),
//											.q_a(temp_read),
//											.q_b(reg_value));

											
always_ff @(posedge CLK) 
begin
	if(RESET)
	begin
		for (int i=0; i < 50; i++)
		begin
			table_val[i] = 32'h0;
		end
	end
	if (AVL_CS & AVL_READ)
	begin
		AVL_READDATA <= table_val[AVL_ADDR];
	end
	if (AVL_CS & AVL_WRITE & (~AVL_ADDR[8]))
 		begin
 			unique case (AVL_BYTE_EN)
 			4'b1111: table_val[AVL_ADDR[7:0]] <= AVL_WRITEDATA;
 			4'b1100: table_val[AVL_ADDR[7:0]][31:16]  <= AVL_WRITEDATA[31:16];
 			4'b0011: table_val[AVL_ADDR[7:0]][15:0]  <= AVL_WRITEDATA[15:0];	
 			4'b1000: table_val[AVL_ADDR[7:0]][31:24]  <= AVL_WRITEDATA[31:24];
 			4'b0100: table_val[AVL_ADDR[7:0]][23:16]  <= AVL_WRITEDATA[23:16];
 			4'b0010: table_val[AVL_ADDR[7:0]][15:8]  <= AVL_WRITEDATA[15:8];
 			4'b0001: table_val[AVL_ADDR[7:0]][7:0]  <= AVL_WRITEDATA[7:0];
 			default:
				table_val[AVL_ADDR[7:0]] <= table_val[AVL_ADDR[7:0]];
 			endcase
 		end

end

											
											
//  // 端口 A 读取逻辑
//  always @(posedge CLK) begin
//
//    if (AVL_READ) begin
//        temp_read <= table_val[AVL_ADDR[10:0]];
//    end
//    if (AVL_WRITE &(~AVL_ADDR[11])) begin
//      if (AVL_BYTE_EN[0])
//        table_val[AVL_ADDR[10:0]][7:0] <= temp_write[7:0];
//      if (AVL_BYTE_EN[1])
//        table_val[AVL_ADDR[10:0]][15:8] <= temp_write[15:8];
//      if (AVL_BYTE_EN[2])
//        table_val[AVL_ADDR[10:0]][23:16] <= temp_write[23:16];
//      if (AVL_BYTE_EN[3])
//        table_val[AVL_ADDR[10:0]][31:24] <= temp_write[31:24];
//    end
//	
////    if (reg_addr[10:0] >= 0 && reg_addr[10:0] + 2 <= 200) begin
////      for (int i = 0; i < 2; i = i + 1) begin
////        table_val[i] <= table_ele[reg_addr[10:0] + i];
////      end
////    end
//  end
//							
////	
//											
//											
//											
//											
////*****************************************************************
//
// 
//// 
////always_comb begin
////	if(AVL_CS) begin
////		temp_write = AVL_WRITEDATA;
////		AVL_READDATA = temp_read;
////	
////	end
////	else begin
////		temp_write = 32'bX;
////		AVL_READDATA = 32'bX;
////	end
////
////end
// 
// 
always_ff @(posedge CLK)
begin
	if (AVL_CS & AVL_WRITE & AVL_ADDR[8])
		begin
			unique case (AVL_BYTE_EN)
			4'b1111: Palette_table[AVL_ADDR[2:0]] <= AVL_WRITEDATA;
			4'b1100: Palette_table[AVL_ADDR[2:0]][31:16]  <= AVL_WRITEDATA[31:16];
			4'b0011: Palette_table[AVL_ADDR[2:0]][15:0]  <= AVL_WRITEDATA[15:0];	
			4'b1000: Palette_table[AVL_ADDR[2:0]][31:24]  <= AVL_WRITEDATA[31:24];
			4'b0100: Palette_table[AVL_ADDR[2:0]][23:16]  <= AVL_WRITEDATA[23:16];
			4'b0010: Palette_table[AVL_ADDR[2:0]][15:8]  <= AVL_WRITEDATA[15:8];
			4'b0001: Palette_table[AVL_ADDR[2:0]][7:0]  <= AVL_WRITEDATA[7:0];
			default:;
			endcase
		end

end	

//==========================================================================================test
//assign table_val[0] = 32'h20000000;
//assign table_val[1] = 32'h20770000;
//assign table_val[2] = 32'h10000C80;
//assign table_val[3] = 32'h00000000;
//===============================================================================================
//readROM  ROM_0(	.color(table_color[0][3:0]),
//			.exist(table_exist[0]),
//			.table_val(table_val[0]),
////			.clk(pixel_clk),
//			.*
//			);
//
//readROM  ROM_1(	.color(table_color[1][3:0]),
//			.exist(table_exist[1]),
//			.table_val(table_val[1]),
////			.clk(pixel_clk),
//			.*
//			);
//readROM  ROM_2(	.color(table_color[2][3:0]),
//			.exist(table_exist[2]),
//			.table_val(table_val[2]),
////			.clk(pixel_clk),
//			.*
//			);
 



//======================================================================================================================ROM module
// generate block 中生成多个模块实例

		arrowROM arrow( 			.color(table_color[0]),
										.exist(table_exist[0]),
										.table_val(table_rom[0]),
										.*
	);
	
		cursorROM cursor( 			.color(table_color[1]),
										.exist(table_exist[1]),
										.table_val(table_rom[1]),
										.*
	);
	
	genvar p;
	generate
		for (p = 6; p < 16; p++) begin : strength_loop
		strengthROM strength_r (
        // 连接子模块的接口
        .color(table_color[p]),
			.exist(table_exist[p]),
			.table_val(table_rom[p]),
			.*
      );
      
      // 为每个实例指定不同的名字
		end
	
	
	endgenerate
	
	
	
	
	genvar m;
	generate
		for (m = 16; m < 26; m++) begin : charging_loop
		chargingROM charge (
        // 连接子模块的接口
        .color(table_color[m]),
			.exist(table_exist[m]),
			.table_val(table_rom[m]),
			.*
      );
      
      // 为每个实例指定不同的名字
		end
	
	
	endgenerate
	
	
	rocketROM Rocket( 			.color(table_color[27]),
										.exist(table_exist[27]),
										.table_val(table_rom[27]),
										.*
	);
	


	genvar k;
	generate
		for (k = 36; k < 46; k++) begin : blood_loop
		bloodROM blood (
        // 连接子模块的接口
        .color(table_color[k]),
			.exist(table_exist[k]),
			.table_val(table_rom[k]),
			.*
      );
      
      // 为每个实例指定不同的名字
		end
	
	
	endgenerate
	
	
	explodeROM explode( 			.color(table_color[46]),
										.exist(table_exist[46]),
										.table_val(table_rom[46]),
										.*
	);
	
	
	cannonballROM canon_ball( 	.color(table_color[47]),
										.exist(table_exist[47]),
										.table_val(table_rom[47]),
										.*
	);
	
	
	repeoROM red_peo(       .color(table_color[48]),
									.exist(table_exist[48]),
									.table_val(table_rom[48]),
									.*
	);
	
	blpeoROM blue_peo(       .color(table_color[49]),
									.exist(table_exist[49]),
									.table_val(table_rom[49]),
									.*
	);
	
	
	

  genvar i;
  generate
 
    for (i = 50; i < 80; i++) begin : instance_loop
		cracterROM instance_name (
        // 连接子模块的接口
        .color(table_color[i]),
			.exist(table_exist[i]),
			.table_val(table_rom[i]),
			.*
      );
      
      // 为每个实例指定不同的名字
		end
  endgenerate
  
	
  
  
  
  //==============================================================================================================================


  // Generate block to generate multiple combinatorial logic units
  generate
    genvar j;
    for (j = 0; j < 120; j = j + 1) begin : logic_unit
      always_comb begin
        unique case (table_val[j][31:26])
		  6'b000000: table_rom[j] = 32'b0;
		  		  
		  default: table_rom[j] = table_val[j];
		  
		  endcase
      end
    end
  endgenerate


  

 
always_ff @(DrawX)
begin
	if (table_exist[0] != 6'b000000)
	begin
	color_index_1 <= table_color[0];
	end
	else if (table_exist[1] != 6'b000000)
	begin
		color_index_1 <= table_color[1];
	end
	else if (table_exist[2] != 6'b000000) begin
	color_index_1 <= table_color[2];
	end
	else if (table_exist[3] != 6'b000000) begin
	color_index_1 <= table_color[3];
	end
	else if (table_exist[4] != 6'b000000) begin
	color_index_1 <= table_color[4];
	end
	else if (table_exist[5] != 6'b000000) begin
	color_index_1 <= table_color[5];
	end
	else
	begin
		color_index_1 <= 4'b1111;
	end
end

always_ff @(DrawX)
begin
	if (table_exist[6] != 6'b000000) begin
	color_index_6 <= table_color[6];
	end
	else if (table_exist[7] != 6'b000000) begin
	color_index_6 <= table_color[7];
	end
	else if (table_exist[8] != 6'b000000) begin
	color_index_6 <= table_color[8];
	end
	else if (table_exist[9] != 6'b000000) begin
	color_index_6 <= table_color[9];
	end
	else if (table_exist[10] != 6'b000000) begin
	color_index_6 <= table_color[10];
	end
	else if (table_exist[11] != 6'b000000) begin
	color_index_6 <= table_color[11];
	end
	else if (table_exist[12] != 6'b000000) begin
	color_index_6 <= table_color[12];
	end
	else if (table_exist[13] != 6'b000000) begin
	color_index_6 <= table_color[13];
	end
	else if (table_exist[14] != 6'b000000) begin
	color_index_6 <= table_color[14];
	end
	else if (table_exist[15] != 6'b000000) begin
	color_index_6 <= table_color[15];
	end
	else
	begin
		color_index_6 <= 4'b1111;
	end
end

always_ff @(DrawX)
begin
	if (table_exist[16] != 6'b000000) begin
	color_index_5 <= table_color[16];
	end
	else if (table_exist[17] != 6'b000000) begin
	color_index_5 <= table_color[17];
	end
	else if (table_exist[18] != 6'b000000) begin
	color_index_5 <= table_color[18];
	end
	else if (table_exist[19] != 6'b000000) begin
	color_index_5 <= table_color[19];
	end
	else if (table_exist[20] != 6'b000000) begin
	color_index_5 <= table_color[20];
	end
	else if (table_exist[21] != 6'b000000) begin
	color_index_5 <= table_color[21];
	end
	else if (table_exist[22] != 6'b000000) begin
	color_index_5 <= table_color[22];
	end
	else if (table_exist[23] != 6'b000000) begin
	color_index_5 <= table_color[23];
	end
	else if (table_exist[24] != 6'b000000) begin
	color_index_5 <= table_color[24];
	end
	else if (table_exist[25] != 6'b000000) begin
	color_index_5 <= table_color[25];
	end
	else
	begin
		color_index_5 <= 4'b1111;
	end
end

always_ff @(DrawX)
begin
	if (table_exist[26] != 6'b000000) begin
	color_index_2 <= table_color[26];
	end
	else if (table_exist[27] != 6'b000000) begin
	color_index_2 <= table_color[27];
	end
	else if (table_exist[28] != 6'b000000) begin
	color_index_2 <= table_color[28];
	end
	else if (table_exist[29] != 6'b000000) begin
	color_index_2 <= table_color[29];
	end
	else if (table_exist[30] != 6'b000000) begin
	color_index_2 <= table_color[30];
	end
	else if (table_exist[31] != 6'b000000) begin
	color_index_2 <= table_color[31];
	end
	else if (table_exist[32] != 6'b000000) begin
	color_index_2 <= table_color[32];
	end
	else if (table_exist[33] != 6'b000000) begin
	color_index_2 <= table_color[33];
	end
	else if (table_exist[34] != 6'b000000) begin
	color_index_2 <= table_color[34];
	end
	else if (table_exist[35] != 6'b000000) begin
	color_index_2 <= table_color[35];
	end
	else if (table_exist[36] != 6'b000000) begin
	color_index_2 <= table_color[36];
	end
	else if (table_exist[37] != 6'b000000) begin
	color_index_2 <= table_color[37];
	end
	else if (table_exist[38] != 6'b000000) begin
	color_index_2 <= table_color[38];
	end
	else if (table_exist[39] != 6'b000000) begin
	color_index_2 <= table_color[39];
	end
	else if (table_exist[40] != 6'b000000) begin
	color_index_2 <= table_color[40];
	end
	else if (table_exist[41] != 6'b000000) begin
	color_index_2 <= table_color[41];
	end
	else if (table_exist[42] != 6'b000000) begin
	color_index_2 <= table_color[42];
	end
	else if (table_exist[43] != 6'b000000) begin
	color_index_2 <= table_color[43];
	end
	else if (table_exist[44] != 6'b000000) begin
	color_index_2 <= table_color[44];
	end
	else if (table_exist[45] != 6'b000000) begin
	color_index_2 <= table_color[45];
	end
	else
	begin
		color_index_2 <= 4'b1111;
	end
end	

always_ff @(DrawX)
begin
	if (table_exist[46] != 6'b000000) begin
	color_index_4 <= table_color[46];
	end
	else if (table_exist[47] != 6'b000000) begin
	color_index_4 <= table_color[47];
	end
	else if (table_exist[48] != 6'b000000) begin
	color_index_4 <= table_color[48];
	end
	else if (table_exist[49] != 6'b000000) begin
	color_index_4 <= table_color[49];
	end
	else
	begin
		color_index_4 <= 4'b1111;
	end
end

always_ff @(DrawX)
begin
	if (table_exist[50] != 6'b000000) begin
	color_index_3 <= table_color[50];
	end
	else if (table_exist[51] != 6'b000000) begin
	color_index_3 <= table_color[51];
	end
	else if (table_exist[52] != 6'b000000) begin
	color_index_3 <= table_color[52];
	end
	else if (table_exist[53] != 6'b000000) begin
	color_index_3 <= table_color[53];
	end
	else if (table_exist[54] != 6'b000000) begin
	color_index_3 <= table_color[54];
	end
	else if (table_exist[55] != 6'b000000) begin
	color_index_3 <= table_color[55];
	end
	else if (table_exist[56] != 6'b000000) begin
	color_index_3 <= table_color[56];
	end
	else if (table_exist[57] != 6'b000000) begin
	color_index_3 <= table_color[57];
	end
	else if (table_exist[58] != 6'b000000) begin
	color_index_3 <= table_color[58];
	end
	else if (table_exist[59] != 6'b000000) begin
	color_index_3 <= table_color[59];
	end
	else if (table_exist[60] != 6'b000000) begin
	color_index_3 <= table_color[60];
	end
	else if (table_exist[61] != 6'b000000) begin
	color_index_3 <= table_color[61];
	end
	else if (table_exist[62] != 6'b000000) begin
	color_index_3 <= table_color[62];
	end
	else if (table_exist[63] != 6'b000000) begin
	color_index_3 <= table_color[63];
	end
	else if (table_exist[64] != 6'b000000) begin
	color_index_3 <= table_color[64];
	end
	else if (table_exist[65] != 6'b000000) begin
	color_index_3 <= table_color[65];
	end
	else if (table_exist[66] != 6'b000000) begin
	color_index_3 <= table_color[66];
	end
	else if (table_exist[67] != 6'b000000) begin
	color_index_3 <= table_color[67];
	end
	else if (table_exist[68] != 6'b000000) begin
	color_index_3 <= table_color[68];
	end
	else if (table_exist[69] != 6'b000000) begin
	color_index_3 <= table_color[69];
	end
	else if (table_exist[70] != 6'b000000) begin
	color_index_3 <= table_color[70];
	end
	else if (table_exist[71] != 6'b000000) begin
	color_index_3 <= table_color[71];
	end
	else if (table_exist[72] != 6'b000000) begin
	color_index_3 <= table_color[72];
	end
	else if (table_exist[73] != 6'b000000) begin
	color_index_3 <= table_color[73];
	end
	else if (table_exist[74] != 6'b000000) begin
	color_index_3 <= table_color[74];
	end
	else if (table_exist[75] != 6'b000000) begin
	color_index_3 <= table_color[75];
	end
	else if (table_exist[76] != 6'b000000) begin
	color_index_3 <= table_color[76];
	end
	else if (table_exist[77] != 6'b000000) begin
	color_index_3 <= table_color[77];
	end
	else if (table_exist[78] != 6'b000000) begin
	color_index_3 <= table_color[78];
	end
	else if (table_exist[79] != 6'b000000) begin
	color_index_3 <= table_color[79];
	end
	else
	begin
	color_index_3 <= 4'b1111;
	end
end

 
 always_ff @(DrawX)
 begin
	if(color_index_1 != 4'b1111)
	begin
		color_index <= color_index_1;
	end
	else if (color_index_6 != 4'b1111) begin
		color_index <= color_index_6;
	end
	else if (color_index_5 != 4'b1111) begin
		color_index <= color_index_5;
	end
	else if (color_index_2 != 4'b1111) begin
		color_index <= color_index_2;
	end
	else if (color_index_4 != 4'b1111) begin
		color_index <= color_index_4;
	end
	else if (color_index_3 != 4'b1111) begin
		color_index <= color_index_3;
	end
	else
	begin
	if(DrawY > 8'b11110000)
	begin
	color_index <= 4'b0111;
	end
	else
	begin
	color_index <= 4'b0011;
	end
	end
 end
 
 
//==============================================================================================

//
//always_comb
//begin
//	if(DrawY < 9'b00001110)
//	begin
//	color_index = 4'b0000;
//	end
//	else if(DrawY < 9'b00011100)
//	begin
//	color_index = 4'b0001;
//	end
//	else if(DrawY < 9'b001011010)
//	begin
//	color_index = 4'b0010;
//	end
//	else if(DrawY < 9'b001111000)
//	begin
//	color_index = 4'b0011;
//	end
//	else
//	begin
//	color_index = 4'b0110;
//	end
//
//
//end



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
	if (color_index[0] )
		begin
			red <= Palette_table[color_index[3:1]][24:21];
			green <= Palette_table[color_index[3:1]][20:17];
			blue <= Palette_table[color_index[3:1]][16:13];
		end
		else
		begin
			red <= Palette_table[color_index[3:1]][12:9];
			green <= Palette_table[color_index[3:1]][8:5];
			blue <= Palette_table[color_index[3:1]][4:1];
		end
	end
 end
 






endmodule




