module custom_apb_buzzer #(
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

  // Buzzer output
  output wire                    buzzerOut
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
reg                      data_buzzer;
wire                     wr_sel;
  
  // Address decoding for write operations
  assign wr_sel = ((PADDR[(ADDRWIDTH-1):2]==10'b00)&(write_en)) ? 1'b1: 1'b0;

  // Write
  always @ (posedge PCLK or negedge PRESETn)
    begin
    if (~PRESETn)
      begin
        data_buzzer <= 1'b0; // Reset data_buzzer to 0x00000000
      end
    else if (wr_sel)
      begin
        data_buzzer <= PWDATA[0];
      end
    end

  // Read
  always @ (read_en)
    begin
      case (PADDR[(ADDRWIDTH-1):2])
        10'b00: rdata <=  {{31{1'b0}},data_buzzer};
        default: rdata <= {32{1'b0}};  // default
      endcase
    end

assign PRDATA = rdata;

assign buzzerOut = data_buzzer;

endmodule
