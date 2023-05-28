
module strengthROM(
                input logic [9:0] DrawX,DrawY,
                input logic [31:0] table_val,
                output logic [3:0] color,
                output logic [5:0] exist
);


    reg [3:0] un_strength[0:550];
    reg [3:0] strength_array[0:550];

	
	


	//***********************************************************************************
	//initial part
	initial
	begin

        $readmemh("sprite_bytes/FilledstrengthUnit.txt", strength_array); 
        $readmemh("sprite_bytes/UnfilledstrengthUnit.txt", un_strength); 
	end

   always_ff @(DrawX)
	begin
        unique case (table_val[31:26])
        6'b010101:
        begin
            if((DrawX >=((table_val[23:14] >= 32)?(table_val[23:14] - 32):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 16))  )
				begin
				if((~strength_array[(DrawY-table_val[13:4])*32 + (DrawX + 32 -table_val[23:14] )]))
				begin
						color <= strength_array[(DrawY-table_val[13:4])*32 + (DrawX + 32 -table_val[23:14])];
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

        6'b010110:
        begin
            if((DrawX >=((table_val[23:14] >= 32)?(table_val[23:14] - 32):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 16))  )
				begin
				if((~un_strength[(DrawY-table_val[13:4])*32 + (DrawX + 32-table_val[23:14] )]))
				begin
						color <= un_strength[(DrawY-table_val[13:4])*32 + (DrawX + 32 -table_val[23:14])];
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
                    color <= 4'b0011;
					exist <= 6'b0;
        end

        endcase
		
			
				
	end

endmodule