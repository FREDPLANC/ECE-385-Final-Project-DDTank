
module cracterROM(
                input logic [9:0] DrawX,DrawY,
                input logic [31:0] table_val,
                output logic [3:0] color,
                output logic [5:0] exist
);


	reg [3:0] cracter [0:4000]; // OCM 内存数组

	reg cracter_co [0:4000]; // OCM 内存数组
	
	


	//***********************************************************************************
	//initial part
	initial
	begin


		 $readmemh("sprite_bytes/cracter.txt", cracter);
 
		 $readmemb("sprite_bytes/cracterColide.txt", cracter_co); 
	end

   always_ff @(DrawX)
	begin

		if(table_val[31:26] != 0)
			begin
			 if((DrawX >=((table_val[23:14] >= 81)?(table_val[23:14] - 81):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 49))  )
				begin
				if((~cracter_co[(DrawY-table_val[13:4])*81 + (DrawX + 81 -table_val[23:14] )]))
				begin
						color <= cracter[(DrawY-table_val[13:4])*81 + (DrawX + 81 -table_val[23:14])];
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