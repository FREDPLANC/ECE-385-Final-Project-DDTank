//-------------------------------------------------------------------------
//      lab9.sv                                                          --
//      Christine Chen                                                   --
//      Fall 2014                                                        --
//                                                                       --
//      Modified by Po-Han Huang                                         --
//      10/06/2017                                                       --
//      Zuofu Cheng                                                      --
//      4/10/2023                                                        --
//      Spring 2023 Distribution                                         --
//                                                                       --
//      For use with ECE 385 Lab 9 (DE2-115)                             --
//      UIUC ECE Department                                              --
//-------------------------------------------------------------------------


module lab9( input               CLOCK_50,
             input        [3:0]  KEY,          //bit 0 is set up as Reset
             output logic [6:0]  HEX0, HEX1,
             // VGA Interface 
             output logic [7:0]  VGA_R,        //VGA Red
                                 VGA_G,        //VGA Green
                                 VGA_B,        //VGA Blue
             output logic        VGA_CLK,      //VGA Clock
                                 VGA_SYNC_N,   //VGA Sync signal
                                 VGA_BLANK_N,  //VGA Blank signal
                                 VGA_VS,       //VGA virtical sync signal
                                 VGA_HS,       //VGA horizontal sync signal
            
				 // CY7C67200 Interface
             inout  wire  [15:0] OTG_DATA,     //CY7C67200 Data bus 16 Bits
             output logic [1:0]  OTG_ADDR,     //CY7C67200 Address 2 Bits
             output logic        OTG_CS_N,     //CY7C67200 Chip Select
                                 OTG_RD_N,     //CY7C67200 Write
                                 OTG_WR_N,     //CY7C67200 Read
                                 OTG_RST_N,    //CY7C67200 Reset
             input               OTG_INT,      //CY7C67200 Interrupt
				 
             // SDRAM Interface for Nios II Software
             output logic [12:0] DRAM_ADDR,    //SDRAM Address 13 Bits
             inout  wire  [31:0] DRAM_DQ,      //SDRAM Data 32 Bits
             output logic [1:0]  DRAM_BA,      //SDRAM Bank Address 2 Bits
             output logic [3:0]  DRAM_DQM,     //SDRAM Data Mast 4 Bits
             output logic        DRAM_RAS_N,   //SDRAM Row Address Strobe
                                 DRAM_CAS_N,   //SDRAM Column Address Strobe
                                 DRAM_CKE,     //SDRAM Clock Enable
                                 DRAM_WE_N,    //SDRAM Write Enable
                                 DRAM_CS_N,    //SDRAM Chip Select
                                 DRAM_CLK      //SDRAM Clock
				);
    
    logic Reset_h, Clk;
	 
	 logic [7:0] keycode;
	 logic [1:0] hpi_addr;
    logic [15:0] hpi_data_in, hpi_data_out;
    logic hpi_r, hpi_w, hpi_cs, hpi_reset; 
    
    assign Clk = CLOCK_50;
    always_ff @ (posedge Clk) begin
        Reset_h <= ~(KEY[0]);        // The push buttons are active low
    end
   
     // You need to make sure that the port names here match the ports in Qsys-generated codes.
	hpi_io_intf hpi_io_inst(
                            .Clk(Clk),
                            .Reset(Reset_h),
                            // signals connected to NIOS II
                            .from_sw_address(hpi_addr),
                            .from_sw_data_in(hpi_data_in),
                            .from_sw_data_out(hpi_data_out),
                            .from_sw_r(hpi_r),
                            .from_sw_w(hpi_w),
                            .from_sw_cs(hpi_cs),
                            .from_sw_reset(hpi_reset),
                            // signals connected to EZ-OTG chip
                            .OTG_DATA(OTG_DATA),    
                            .OTG_ADDR(OTG_ADDR),    
                            .OTG_RD_N(OTG_RD_N),    
                            .OTG_WR_N(OTG_WR_N),    
                            .OTG_CS_N(OTG_CS_N),
                            .OTG_RST_N(OTG_RST_N)
    ); 
	
	lab9_soc u0 (
					 .clk_clk                           (Clk),      //clk.clk
					 .reset_reset_n(1'b1),    						// Never reset NIOS
					 .sdram_wire_addr(DRAM_ADDR), 
					 .sdram_wire_ba(DRAM_BA),   
					 .sdram_wire_cas_n(DRAM_CAS_N),
					 .sdram_wire_cke(DRAM_CKE),  
					 .sdram_wire_cs_n(DRAM_CS_N), 
					 .sdram_wire_dq(DRAM_DQ),   
					 .sdram_wire_dqm(DRAM_DQM),  
					 .sdram_wire_ras_n(DRAM_RAS_N),
					 .sdram_wire_we_n(DRAM_WE_N), 
					 .sdram_clk_clk(DRAM_CLK),
					 //USB input
					 .keycode_export(keycode),  
					 .otg_hpi_address_export(hpi_addr),
					 .otg_hpi_data_in_port(hpi_data_in),
					 .otg_hpi_data_out_port(hpi_data_out),
					 .otg_hpi_cs_export(hpi_cs),
					 .otg_hpi_r_export(hpi_r),
					 .otg_hpi_w_export(hpi_w),
					 .otg_hpi_reset_export(hpi_reset),	
					//VGA
					.vga_port_red (VGA_R[7:4]),
					.vga_port_green (VGA_G[7:4]),
					.vga_port_blue (VGA_B[7:4]),
					.vga_port_hs (VGA_HS),
					.vga_port_vs (VGA_VS),
					.vga_port_pixel_clk (VGA_CLK),
					.vga_port_blank (VGA_BLANK_N),
					.vga_port_sync (VGA_SYNC_N)
    );
	 
	 assign VGA_R[3:0] = 4'h0;
	 assign VGA_G[3:0] = 4'h0;
	 assign VGA_B[3:0] = 4'h0;
//	 assign HEX0 = 7'hFF;
//	 assign HEX1 = 7'hFF;
	 HexDriver hex_inst_0 (keycode[3:0], HEX0);
    HexDriver hex_inst_1 (keycode[7:4], HEX1);
	 
endmodule
