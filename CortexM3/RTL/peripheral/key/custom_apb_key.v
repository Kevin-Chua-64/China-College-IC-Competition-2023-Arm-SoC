module custom_apb_key #(
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

  // Key input
  input wire   [3:0]             keyIn
 );


// APB interface
assign   PREADY  = 1'b1; //always ready. Can be customized to support waitstate if required.
assign   PSLVERR = 1'b0; //alwyas OKAY. Can be customized to support error response if required.

//------------------------------------------------------------------------------
// internal signals
//------------------------------------------------------------------------------
assign  read_en  = PSEL & (~PWRITE); // assert for whole apb read transfer
reg    [31:0]            rdata;		  

	always @ (posedge PCLK or negedge PRESETn)
	begin
		if(~PRESETn) begin
			rdata <= {32{1'b0}};
		end else if (read_en) begin
			case (PADDR[(ADDRWIDTH-1):2])
				10'b00: rdata <= {{28{1'b0}},keyIn};
			endcase
		end
	end	

assign PRDATA = rdata;

endmodule
