module OCMModule (
  input wire clk,
  input wire [10:0] a_addr,
  input wire [10:0] b_addr,
  input wire a_read_enable,
  input wire a_write_enable,
  input wire [3:0] a_byte_enable,
  input wire [31:0] a_data_in,
  input wire [10:0] num,
  output wire [31:0] a_data_out,
  output wire [31:0] b_data_out [2]
);

  reg [31:0] mem [10:0]; // 存储器数组，大小为 1500 个 32 位单元

  // 端口 A 读取逻辑
  always @(posedge clk) begin
    if (a_read_enable) begin
      if (a_byte_enable[0])
        a_data_out[7:0] = mem[a_addr][7:0];
      if (a_byte_enable[1])
        a_data_out[15:8] = mem[a_addr][15:8];
      if (a_byte_enable[2])
        a_data_out[23:16] = mem[a_addr][23:16];
      if (a_byte_enable[3])
        a_data_out[31:24] = mem[a_addr][31:24];
    end
  end

  // 端口 A 写入逻辑
  always @(posedge clk) begin
    if (a_write_enable) begin
      if (a_byte_enable[0])
        mem[a_addr][7:0] = a_data_in[7:0];
      if (a_byte_enable[1])
        mem[a_addr][15:8] = a_data_in[15:8];
      if (a_byte_enable[2])
        mem[a_addr][23:16] = a_data_in[23:16];
      if (a_byte_enable[3])
        mem[a_addr][31:24] = a_data_in[31:24];
    end
  end

  // B 端口读取逻辑
  always @(posedge clk) begin
    if (b_addr >= 0 && b_addr + num <= 1499) begin
      for (int i = 0; i < num; i = i + 1) begin
        b_data_out[i] = mem[b_addr + i];
      end
    end
  end

endmodule
