
module rocketROM(
                input logic [9:0] DrawX,DrawY,
                input logic [31:0] table_val,
                output logic [3:0] color,
                output logic [5:0] exist
);


	reg [3:0] rocket [0:500]; // OCM 内存数组

	reg rocket_co [0:500];


	//***********************************************************************************
	//initial part
	initial
	begin


		 $readmemh("sprite_bytes/rocket.txt", rocket);
		 $readmemb("sprite_bytes/rocketColide.txt", rocket_co);
 
	end

   always_ff @(DrawX)
	begin

		if(table_val[31:26] != 0)
			begin
			 if((DrawX >=((table_val[23:14] >= 18)?(table_val[23:14] - 18):0)) && (DrawX < (table_val[23:14])) && (DrawY >=table_val[13:4]) && (DrawY < (table_val[13:4] + 26))  )
				begin
				if((~rocket_co[(DrawY-table_val[13:4])*18 + (DrawX + 18 -table_val[23:14] )]))
				begin
						color <= rocket[(DrawY-table_val[13:4])*18 + (DrawX + 18 -table_val[23:14])];
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