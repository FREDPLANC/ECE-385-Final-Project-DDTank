
module blpeoROM(
                input logic [9:0] DrawX,DrawY,
                input logic [31:0] table_val,
                output logic [3:0] color,
                output logic [5:0] exist
);
			 
	reg [3:0] Blpeo [0:450]; // OCM 内存数组
	reg [3:0] Brpeo [0:450]; // OCM 内存数组

	reg [3:0] dLeftB [0:1100]; // OCM 内存数组
	reg [3:0] dRightB [0:1100]; // OCM 内存数组
	reg [3:0] uLeftB [0:1100]; // OCM 内存数组
	reg [3:0] uRightB [0:1100]; // OCM 内存数组


	reg Left_co [0:450]; // OCM 内存数组
	reg Right_co [0:450]; // OCM 内存数组

	reg downLeft_co [0:1100]; // OCM 内存数组
	reg downRight_co [0:1100]; // OCM 内存数组
	reg upLeft_co [0:1100]; // OCM 内存数组
	reg upRight_co [0:1100]; // OCM 内存数组
	


	//***********************************************************************************
	//initial part
	initial
	begin
		$readmemh("sprite_bytes/blueBl.txt", Blpeo); 
		$readmemh("sprite_bytes/blueBr.txt", Brpeo);

    	$readmemh("sprite_bytes/uphillLeftBlue.txt", uLeftB);
		$readmemh("sprite_bytes/uphillRightBlue.txt", uRightB);
		$readmemh("sprite_bytes/downhillRightBlue.txt", dRightB);
		$readmemh("sprite_bytes/downhillLeftBlue.txt", dLeftB); 



		$readmemb("sprite_bytes/LlColide.txt", Left_co); 
		$readmemb("sprite_bytes/RrColide.txt", Right_co);
		$readmemb("sprite_bytes/downhillLeftColide.txt", downLeft_co); 
		$readmemb("sprite_bytes/downhillRightColide.txt", downRight_co); 
		$readmemb("sprite_bytes/uphillLeftColide.txt", upLeft_co); 
		$readmemb("sprite_bytes/uphillRightColide.txt", upRight_co); 
		 
	end

  always_ff @(DrawX)
	begin

			unique case (table_val[31:26])
			6'b000000:
			begin
			color <= 4'b0100;
			exist <= 6'b0;
			end


			// blue player
			6'b000100:		
			begin
			if((DrawX >=((table_val[23:14] >= 21)?(table_val[23:14] - 21):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 21))  )
				begin
				if((~Left_co[(DrawY-table_val[13:4])*21 + (DrawX + 21 -table_val[23:14] )]))
				begin
						color <= Blpeo[(DrawY-table_val[13:4])*21 + (DrawX + 21 -table_val[23:14])];
						exist <= table_val[31:26];
				end
				else
				begin
						color <= 4'b0011;
						exist <= 6'b0;
				end
				end
				else 
				begin
					//temp_write_2 = 32'h0000;
						color <= 4'b0011;
						exist <= 6'b0;
				end
			end
			6'b000101:		
			begin
			if((DrawX >=((table_val[23:14] >= 21)?(table_val[23:14] - 21):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 21))  )
				begin
				if((~Right_co[(DrawY-table_val[13:4])*21 + (DrawX + 21 -table_val[23:14] )]))
				begin
						color <= Brpeo[(DrawY-table_val[13:4])*21 + (DrawX +21 -table_val[23:14])];
						exist <= table_val[31:26];
				end
				else
				begin
						color <= 4'b0011;
						exist <= 6'b0;
				end
				end
				else 
				begin
					//temp_write_2 = 32'h0000;
						color <= 4'b0011;
						exist <= 6'b0;
				end
			end
			6'b000110:		
			begin
			if((DrawX >=((table_val[23:14] >= 33)?(table_val[23:14] - 33):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 33))  )
				begin
				if((~upLeft_co[(DrawY-table_val[13:4])*33 + (DrawX + 33-table_val[23:14] )]))
				begin
						color <= uLeftB[(DrawY-table_val[13:4])*33 + (DrawX + 33 -table_val[23:14])];
						exist <= table_val[31:26];
				end
				else
				begin
						color <= 4'b0011;
						exist <= 6'b0;
				end
				end
				else 
				begin
					//temp_write_2 = 32'h0000;
						color <= 4'b0011;
						exist <= 6'b0;
				end
			end
			6'b000111:		
			begin
			if((DrawX >=((table_val[23:14] >= 33)?(table_val[23:14] - 33):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 33))  )
				begin
				if((~upRight_co[(DrawY-table_val[13:4])*33 + (DrawX + 33 -table_val[23:14] )]))
				begin
						color <= uRightB[(DrawY-table_val[13:4])*33 + (DrawX + 33 -table_val[23:14])];
						exist <= table_val[31:26];
				end
				else
				begin
						color <= 4'b0011;
						exist <= 6'b0;
				end
				end
				else 
				begin
					//temp_write_2 = 32'h0000;
						color <= 4'b0011;
						exist <= 6'b0;
				end
			end
			6'b001000:		
			begin
			if((DrawX >=((table_val[23:14] >= 33)?(table_val[23:14] - 33):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 33))  )
				begin
				if((~downLeft_co[(DrawY-table_val[13:4])*33 + (DrawX + 33 -table_val[23:14] )]))
				begin
						color <= dLeftB[(DrawY-table_val[13:4])*33 + (DrawX + 33 -table_val[23:14])];
						exist <= table_val[31:26];
				end
				else
				begin
						color <= 4'b0011;
						exist <= 6'b0;
				end
				end
				else 
				begin
					//temp_write_2 = 32'h0000;
						color <= 4'b0011;
						exist <= 6'b0;
				end
			end
			6'b001001:		
			begin
			if((DrawX >=((table_val[23:14] >= 33)?(table_val[23:14] - 33):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 33))  )
				begin
				if((~downRight_co[(DrawY-table_val[13:4])*33 + (DrawX + 33 -table_val[23:14] )]))
				begin
						color <= dRightB[(DrawY-table_val[13:4])*33 + (DrawX + 33 -table_val[23:14])];
						exist <= table_val[31:26];
				end
				else
				begin
						color <= 4'b0011;
						exist <= 6'b0;
				end
				end
				else 
				begin
					//temp_write_2 = 32'h0000;
						color <= 4'b0011;
						exist <= 6'b0;
				end
			end
			
			default:	
				begin 
				color <= 4'b0100;
				exist <= 6'b0;
				end
			endcase
	end



endmodule