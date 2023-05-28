
module bloodROM(
                input logic [9:0] DrawX,DrawY,
                input logic [31:0] table_val,
                output logic [3:0] color,
                output logic [5:0] exist
);


	reg [3:0] un_Funit [0:910];
   reg [3:0] Funit [0:910];

	reg Funit_co [0:910];
	


	//***********************************************************************************
	//initial part
	initial
	begin

        $readmemh("sprite_bytes/FilledhealthUnit.txt", Funit); 
        $readmemh("sprite_bytes/UnfilledhealthUnit.txt", un_Funit); 
			$readmemh("sprite_bytes/healthUnitColide.txt", Funit_co); 
	end

   always_ff @(DrawX)
	begin
        unique case (table_val[31:26])
        6'b010001:
        begin
            if((DrawX >=((table_val[23:14] >= 30)?(table_val[23:14] - 30):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 30))  )
				begin
				if((~Funit_co[(DrawY-table_val[13:4])*30 + (DrawX + 30 -table_val[23:14] )]))
				begin
						color <= Funit[(DrawY-table_val[13:4])*30 + (DrawX + 30 -table_val[23:14])];
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

        6'b010010:
        begin
            if((DrawX >=((table_val[23:14] >= 30)?(table_val[23:14] - 30):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 30))  )
				begin
				if((~Funit_co[(DrawY-table_val[13:4])*30 + (DrawX + 30-table_val[23:14] )]))
				begin
						color <= un_Funit[(DrawY-table_val[13:4])*30 + (DrawX + 30 -table_val[23:14])];
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