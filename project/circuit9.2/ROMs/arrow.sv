module arrowROM(
                input logic [9:0] DrawX,DrawY,
                input logic [31:0] table_val,
                output logic [3:0] color,
                output logic [5:0] exist
);


	reg [3:0] arrow [0:90]; // OCM 内存数组

	reg arrow_co [0:90]; // OCM 内存数组
	
	


	//***********************************************************************************
	//initial part
	initial
	begin


		 $readmemh("sprite_bytes/arrow.txt", arrow);
 
		 $readmemb("sprite_bytes/arrowColide.txt", arrow_co); 
	end

   always_ff @(DrawX)
	begin
		
		if(table_val[31:26] != 0)
		begin
		if((DrawX >=((table_val[23:14] >= 9)?(table_val[23:14] - 9):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 9))  )
				begin
				if((~arrow_co[(DrawY-table_val[13:4])*9 + (DrawX + 9 -table_val[23:14] )]))
				begin
						color <= arrow[(DrawY-table_val[13:4])*9 + (DrawX + 9 -table_val[23:14])];
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
		else
			begin
			color <= 4'b0100;
			exist <= 6'b0;
			end			
	end

endmodule