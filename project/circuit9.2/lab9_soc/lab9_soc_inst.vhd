	component lab9_soc is
		port (
			clk_clk                : in    std_logic                     := 'X';             -- clk
			keycode_export         : out   std_logic_vector(7 downto 0);                     -- export
			otg_hpi_address_export : out   std_logic_vector(1 downto 0);                     -- export
			otg_hpi_cs_export      : out   std_logic;                                        -- export
			otg_hpi_data_in_port   : in    std_logic_vector(15 downto 0) := (others => 'X'); -- in_port
			otg_hpi_data_out_port  : out   std_logic_vector(15 downto 0);                    -- out_port
			otg_hpi_r_export       : out   std_logic;                                        -- export
			otg_hpi_reset_export   : out   std_logic;                                        -- export
			otg_hpi_w_export       : out   std_logic;                                        -- export
			reset_reset_n          : in    std_logic                     := 'X';             -- reset_n
			sdram_clk_clk          : out   std_logic;                                        -- clk
			sdram_wire_addr        : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_wire_ba          : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_wire_cas_n       : out   std_logic;                                        -- cas_n
			sdram_wire_cke         : out   std_logic;                                        -- cke
			sdram_wire_cs_n        : out   std_logic;                                        -- cs_n
			sdram_wire_dq          : inout std_logic_vector(31 downto 0) := (others => 'X'); -- dq
			sdram_wire_dqm         : out   std_logic_vector(3 downto 0);                     -- dqm
			sdram_wire_ras_n       : out   std_logic;                                        -- ras_n
			sdram_wire_we_n        : out   std_logic;                                        -- we_n
			vga_port_blank         : out   std_logic;                                        -- blank
			vga_port_blue          : out   std_logic_vector(3 downto 0);                     -- blue
			vga_port_green         : out   std_logic_vector(3 downto 0);                     -- green
			vga_port_hs            : out   std_logic;                                        -- hs
			vga_port_pixel_clk     : out   std_logic;                                        -- pixel_clk
			vga_port_red           : out   std_logic_vector(3 downto 0);                     -- red
			vga_port_sync          : out   std_logic;                                        -- sync
			vga_port_vs            : out   std_logic                                         -- vs
		);
	end component lab9_soc;

	u0 : component lab9_soc
		port map (
			clk_clk                => CONNECTED_TO_clk_clk,                --             clk.clk
			keycode_export         => CONNECTED_TO_keycode_export,         --         keycode.export
			otg_hpi_address_export => CONNECTED_TO_otg_hpi_address_export, -- otg_hpi_address.export
			otg_hpi_cs_export      => CONNECTED_TO_otg_hpi_cs_export,      --      otg_hpi_cs.export
			otg_hpi_data_in_port   => CONNECTED_TO_otg_hpi_data_in_port,   --    otg_hpi_data.in_port
			otg_hpi_data_out_port  => CONNECTED_TO_otg_hpi_data_out_port,  --                .out_port
			otg_hpi_r_export       => CONNECTED_TO_otg_hpi_r_export,       --       otg_hpi_r.export
			otg_hpi_reset_export   => CONNECTED_TO_otg_hpi_reset_export,   --   otg_hpi_reset.export
			otg_hpi_w_export       => CONNECTED_TO_otg_hpi_w_export,       --       otg_hpi_w.export
			reset_reset_n          => CONNECTED_TO_reset_reset_n,          --           reset.reset_n
			sdram_clk_clk          => CONNECTED_TO_sdram_clk_clk,          --       sdram_clk.clk
			sdram_wire_addr        => CONNECTED_TO_sdram_wire_addr,        --      sdram_wire.addr
			sdram_wire_ba          => CONNECTED_TO_sdram_wire_ba,          --                .ba
			sdram_wire_cas_n       => CONNECTED_TO_sdram_wire_cas_n,       --                .cas_n
			sdram_wire_cke         => CONNECTED_TO_sdram_wire_cke,         --                .cke
			sdram_wire_cs_n        => CONNECTED_TO_sdram_wire_cs_n,        --                .cs_n
			sdram_wire_dq          => CONNECTED_TO_sdram_wire_dq,          --                .dq
			sdram_wire_dqm         => CONNECTED_TO_sdram_wire_dqm,         --                .dqm
			sdram_wire_ras_n       => CONNECTED_TO_sdram_wire_ras_n,       --                .ras_n
			sdram_wire_we_n        => CONNECTED_TO_sdram_wire_we_n,        --                .we_n
			vga_port_blank         => CONNECTED_TO_vga_port_blank,         --        vga_port.blank
			vga_port_blue          => CONNECTED_TO_vga_port_blue,          --                .blue
			vga_port_green         => CONNECTED_TO_vga_port_green,         --                .green
			vga_port_hs            => CONNECTED_TO_vga_port_hs,            --                .hs
			vga_port_pixel_clk     => CONNECTED_TO_vga_port_pixel_clk,     --                .pixel_clk
			vga_port_red           => CONNECTED_TO_vga_port_red,           --                .red
			vga_port_sync          => CONNECTED_TO_vga_port_sync,          --                .sync
			vga_port_vs            => CONNECTED_TO_vga_port_vs             --                .vs
		);

