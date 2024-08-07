module custom_apb_lcd #(
  // parameter for address width
  parameter ADDRWIDTH = 12)
 (
  // IO declaration
  input  wire                    PCLK,     // pclk
  input  wire                    PRESETn,  // reset

  // apb interface inputs
  input  wire                    PSEL,
  input  wire  [ADDRWIDTH-1:0]   PADDR,
  input  wire                    PENABLE,
  input  wire                    PWRITE,
  input  wire  [31:0]            PWDATA,

  // Engineering-change-order revision bits
  input  wire  [3:0]             ECOREVNUM,

  // apb interface outputs
  output wire  [31:0]            PRDATA,
  output wire                    PREADY,
  output wire                    PSLVERR,

  // LCD output
  output  wire                   LCD_CS,
  output  wire                   LCD_RS,
  output  wire                   LCD_WR,
  output  wire                   LCD_RD,
  output  wire                   LCD_RST,
  output  wire                   LCD_BL_CTR,
  output  wire [15:0]            LCD_DATA
 );
  
  
// APB interface
assign   PREADY  = 1'b1; //always ready. Can be customized to support waitstate if required.
assign   PSLVERR = 1'b0; //alwyas OKAY. Can be customized to support error response if required.

//------------------------------------------------------------------------------
// internal signals
//------------------------------------------------------------------------------
assign  read_en  = PSEL & (~PWRITE); // assert for whole apb read transfer
assign  write_en = PSEL & (~PENABLE) & PWRITE; // assert for 1st cycle of write transfer
        // It is also possible to change the design to perform the write in the 2nd
        // APB cycle.   E.g.
        //   assign write_en = PSEL & PENABLE & PWRITE;
        // However, if the design generate waitstate, this expression will result
        // in write_en being asserted for multiple cycles.
		  
reg    [31:0]            rdata;		  
reg                      data_LCD_CS;
reg                      data_LCD_RS;
reg                      data_LCD_WR;
reg                      data_LCD_RD;
reg                      data_LCD_RST;
reg                      data_LCD_BL_CTR;
reg    [15:0]            data_LCD_DATA;
wire   [21:0]            wr_sel;
  
  // Address decoding for write operations
  assign wr_sel[0] = ((PADDR[(ADDRWIDTH-1):2]==10'h00)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[1] = ((PADDR[(ADDRWIDTH-1):2]==10'h01)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[2] = ((PADDR[(ADDRWIDTH-1):2]==10'h02)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[3] = ((PADDR[(ADDRWIDTH-1):2]==10'h03)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[4] = ((PADDR[(ADDRWIDTH-1):2]==10'h04)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[5] = ((PADDR[(ADDRWIDTH-1):2]==10'h05)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[6] = ((PADDR[(ADDRWIDTH-1):2]==10'h06)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[7] = ((PADDR[(ADDRWIDTH-1):2]==10'h07)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[8] = ((PADDR[(ADDRWIDTH-1):2]==10'h08)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[9] = ((PADDR[(ADDRWIDTH-1):2]==10'h09)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[10] = ((PADDR[(ADDRWIDTH-1):2]==10'h0A)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[11] = ((PADDR[(ADDRWIDTH-1):2]==10'h0B)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[12] = ((PADDR[(ADDRWIDTH-1):2]==10'h0C)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[13] = ((PADDR[(ADDRWIDTH-1):2]==10'h0D)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[14] = ((PADDR[(ADDRWIDTH-1):2]==10'h0E)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[15] = ((PADDR[(ADDRWIDTH-1):2]==10'h0F)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[16] = ((PADDR[(ADDRWIDTH-1):2]==10'h10)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[17] = ((PADDR[(ADDRWIDTH-1):2]==10'h11)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[18] = ((PADDR[(ADDRWIDTH-1):2]==10'h12)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[19] = ((PADDR[(ADDRWIDTH-1):2]==10'h13)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[20] = ((PADDR[(ADDRWIDTH-1):2]==10'h14)&(write_en)) ? 1'b1: 1'b0;
  assign wr_sel[21] = ((PADDR[(ADDRWIDTH-1):2]==10'h15)&(write_en)) ? 1'b1: 1'b0;

  // Write
  always @ (posedge PCLK or negedge PRESETn)
    begin
    if (~PRESETn) // Reset to 0x00000000
      begin
			data_LCD_CS     <= 1'b0;
			data_LCD_RS     <= 1'b0;
			data_LCD_WR     <= 1'b0;
			data_LCD_RD     <= 1'b0;
			data_LCD_RST    <= 1'b0;
			data_LCD_BL_CTR <= 1'b0;
			data_LCD_DATA   <= 16'b0;
      end
    else if (wr_sel[0]) begin
        data_LCD_CS <= PWDATA[0];
    end else if (wr_sel[1]) begin
        data_LCD_RS <= PWDATA[0];
    end else if (wr_sel[2]) begin
        data_LCD_WR <= PWDATA[0];
    end else if (wr_sel[3]) begin
        data_LCD_RD <= PWDATA[0];
    end else if (wr_sel[4]) begin
        data_LCD_RST <= PWDATA[0];
    end else if (wr_sel[5]) begin
        data_LCD_BL_CTR <= PWDATA[0];
    end else if (wr_sel[6]) begin
        data_LCD_DATA[0] <= PWDATA[0];
    end else if (wr_sel[7]) begin
        data_LCD_DATA[1] <= PWDATA[0];
    end else if (wr_sel[8]) begin
        data_LCD_DATA[2] <= PWDATA[0];
    end else if (wr_sel[9]) begin
        data_LCD_DATA[3] <= PWDATA[0];
    end else if (wr_sel[10]) begin
        data_LCD_DATA[4] <= PWDATA[0];
    end else if (wr_sel[11]) begin
        data_LCD_DATA[5] <= PWDATA[0];
    end else if (wr_sel[12]) begin
        data_LCD_DATA[6] <= PWDATA[0];
    end else if (wr_sel[13]) begin
        data_LCD_DATA[7] <= PWDATA[0];
    end else if (wr_sel[14]) begin
        data_LCD_DATA[8] <= PWDATA[0];
    end else if (wr_sel[15]) begin
        data_LCD_DATA[9] <= PWDATA[0];
    end else if (wr_sel[16]) begin
        data_LCD_DATA[10] <= PWDATA[0];
    end else if (wr_sel[17]) begin
        data_LCD_DATA[11] <= PWDATA[0];
    end else if (wr_sel[18]) begin
        data_LCD_DATA[12] <= PWDATA[0];
    end else if (wr_sel[19]) begin
        data_LCD_DATA[13] <= PWDATA[0];
    end else if (wr_sel[20]) begin
        data_LCD_DATA[14] <= PWDATA[0];
    end else if (wr_sel[21]) begin
        data_LCD_DATA[15] <= PWDATA[0];
    end
    end

  // Read
  always @ (read_en)
    begin
      case (PADDR[(ADDRWIDTH-1):2])
        10'h00: rdata <=  {{31{1'b0}},data_LCD_CS};
        10'h01: rdata <=  {{31{1'b0}},data_LCD_RS};
		  10'h02: rdata <=  {{31{1'b0}},data_LCD_WR};
		  10'h03: rdata <=  {{31{1'b0}},data_LCD_RD};
		  10'h04: rdata <=  {{31{1'b0}},data_LCD_RST};
		  10'h05: rdata <=  {{31{1'b0}},data_LCD_BL_CTR};
		  10'h06: rdata <=  {{31{1'b0}},data_LCD_DATA[0]};
		  10'h07: rdata <=  {{31{1'b0}},data_LCD_DATA[1]};
		  10'h08: rdata <=  {{31{1'b0}},data_LCD_DATA[2]};
		  10'h09: rdata <=  {{31{1'b0}},data_LCD_DATA[3]};
		  10'h0A: rdata <=  {{31{1'b0}},data_LCD_DATA[4]};
		  10'h0B: rdata <=  {{31{1'b0}},data_LCD_DATA[5]};
		  10'h0C: rdata <=  {{31{1'b0}},data_LCD_DATA[6]};
		  10'h0D: rdata <=  {{31{1'b0}},data_LCD_DATA[7]};
		  10'h0E: rdata <=  {{31{1'b0}},data_LCD_DATA[8]};
		  10'h0F: rdata <=  {{31{1'b0}},data_LCD_DATA[9]};
		  10'h10: rdata <=  {{31{1'b0}},data_LCD_DATA[10]};
		  10'h11: rdata <=  {{31{1'b0}},data_LCD_DATA[11]};
		  10'h12: rdata <=  {{31{1'b0}},data_LCD_DATA[12]};
		  10'h13: rdata <=  {{31{1'b0}},data_LCD_DATA[13]};
		  10'h14: rdata <=  {{31{1'b0}},data_LCD_DATA[14]};
		  10'h15: rdata <=  {{31{1'b0}},data_LCD_DATA[15]};
		  default: rdata <= {32{1'b0}};  // default
      endcase
    end

assign PRDATA = rdata;

assign LCD_CS     = data_LCD_CS;
assign LCD_RS     = data_LCD_RS;
assign LCD_WR     = data_LCD_WR;
assign LCD_RD     = data_LCD_RD;
assign LCD_RST    = data_LCD_RST;
assign LCD_BL_CTR = data_LCD_BL_CTR;
assign LCD_DATA   = data_LCD_DATA;

endmodule
