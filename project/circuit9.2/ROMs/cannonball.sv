
module cannonballROM(
                input logic [9:0] DrawX,DrawY,
                input logic [31:0] table_val,
                output logic [3:0] color,
                output logic [5:0] exist
);


	reg [3:0] cannonball[0:100];
		reg [3:0] enter [0:500]; // OCM 内存数组


	//reg [3:0] VS_array[0: 875];
	
	//sreg [3:0] win_array[0: 1250];


	//***********************************************************************************
	//initial part
	initial
	begin

        $readmemh("sprite_bytes/cannonBall.txt", cannonball); 
		$readmemh("sprite_bytes/enter.txt", enter);
		//$readmemh("sprite_bytes/vs.txt", VS_array); 
		//$readmemh("sprite_bytes/win.txt", win_array); 
	end


		always_ff @(DrawX)
	begin
        unique case (table_val[31:26])
        6'b000010:
        begin
            if((DrawX >=((table_val[23:14] >= 9)?(table_val[23:14] - 9):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 9))  )
				begin
				if((~cannonball[(DrawY-table_val[13:4])*9 + (DrawX + 9 -table_val[23:14] )]))
				begin
						color <= cannonball[(DrawY-table_val[13:4])*9 + (DrawX + 9 -table_val[23:14])];
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

        6'b011010:
        begin
            if((DrawX >=((table_val[23:14] >= 25)?(table_val[23:14] - 25):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 20))  )
				begin
				if((~enter[(DrawY-table_val[13:4])*25 + (DrawX + 25-table_val[23:14] )]))
				begin
						color <= enter[(DrawY-table_val[13:4])*25 + (DrawX + 25 -table_val[23:14])];
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