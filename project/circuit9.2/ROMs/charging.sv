
module chargingROM(
                input logic [9:0] DrawX,DrawY,
                input logic [31:0] table_val,
                output logic [3:0] color,
                output logic [5:0] exist
);


    reg [3:0] un_charge[0:840];
    reg [3:0] charge[0:840];

	
	


	//***********************************************************************************
	//initial part
	initial
	begin

        $readmemh("sprite_bytes/FilledchargingUnit.txt", charge); 
        $readmemh("sprite_bytes/UnfilledchargingUnit.txt", un_charge); 
	end

   always_ff @(DrawX)
	begin
        unique case (table_val[31:26])
        6'b010011:
        begin
             if((DrawX >=((table_val[23:14] >= 25)?(table_val[23:14] - 25):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 33))  )
				begin
				if((~charge[(DrawY-table_val[13:4])*25 + (DrawX + 25 -table_val[23:14] )]))
				begin
						color <= charge[(DrawY-table_val[13:4])*25 + (DrawX + 25 -table_val[23:14])];
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

        6'b010100:
        begin
            if((DrawX >=((table_val[23:14] >= 25)?(table_val[23:14] - 25):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 33))  )
				begin
				if((~un_charge[(DrawY-table_val[13:4])*25 + (DrawX + 25 -table_val[23:14] )]))
				begin
						color <= un_charge[(DrawY-table_val[13:4])*25 + (DrawX + 25 -table_val[23:14])];
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