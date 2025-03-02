module CortexM3 #(
    parameter                   isSim = 0,
    parameter                   isQuartus = 1
)   (
    input       wire            CLK50m,
    input       wire            RSTn,
	 
    // SWD
    inout       wire            SWDIO,
    input       wire            SWCLK,
	 
    // UART
    output      wire            TXD,
    input       wire            RXD,
	 
    // LCD
    output      wire            LCD_CS,
    output      wire            LCD_RS,
    output      wire            LCD_WR,
    output      wire            LCD_RD,
    output      wire            LCD_RST,
    output      wire            LCD_BL_CTR,
    output      wire    [15:0]  LCD_DATA,
	 
    // Key
    input       wire    [3:0]   keyIn,
	 
    // LED
    output      wire    [1:0]   ledOut,
	 
    // Buzzer
    output      wire            buzzerOut
);


//------------------------------------------------------------------------------
// GLOBAL BUF
//------------------------------------------------------------------------------

wire            clk;
wire            swck;

generate 
        if(isSim) begin : SimClock

                assign swck = SWCLK;
                assign clk  = CLK50m;
        
        end else if(isQuartus) begin : SynClockQuartus

                BUFG sw_clk(
                        .inclk                 (SWCLK),
                        .outclk                (swck)
                );
                
                PLL PLL(
					 	      .areset                (~RSTn),
								.inclk0                (CLK50m),
								.c0                    (clk)
                );
        end else begin : SynClockTD
                PH1_LOGIC_BUFG sw_clk(
                        .i                      (SWCLK),
                        .o                      (swck)
                );
                
                PLL PLL(
                        .refclk                 (CLK50m),
                        .reset                  (~RSTn),
                        .clk0_out               (clk)
                );
		  
		  end
endgenerate         


//------------------------------------------------------------------------------
// DEBUG IOBUF 
//------------------------------------------------------------------------------

wire            SWDO;
wire            SWDOEN;
wire            SWDI;

generate
    if(isSim) begin : SimIOBuf

        assign SWDI = SWDIO;
        assign SWDIO = (SWDOEN) ?  SWDO : 1'bz;

    end else begin : SynIOBuf
        
        assign SWDI = SWDIO;
        assign SWDIO = (SWDOEN) ?  SWDO : 1'bz;
        
    end
endgenerate


//------------------------------------------------------------------------------
// RESET
//------------------------------------------------------------------------------

wire            SYSRESETREQ;
reg             cpuresetn;

always @(posedge clk or negedge RSTn)begin
    if (~RSTn) 
        cpuresetn <= 1'b0;
    else if (SYSRESETREQ) 
        cpuresetn <= 1'b0;
    else 
        cpuresetn <= 1'b1;
end

wire        SLEEPing;


//------------------------------------------------------------------------------
// DEBUG CONFIG
//------------------------------------------------------------------------------

wire            CDBGPWRUPREQ;
reg             CDBGPWRUPACK;

always @(posedge clk or negedge RSTn)begin
    if (~RSTn) 
        CDBGPWRUPACK <= 1'b0;
    else 
        CDBGPWRUPACK <= CDBGPWRUPREQ;
end


//------------------------------------------------------------------------------
// INTERRUPT 
//------------------------------------------------------------------------------

wire    [239:0] IRQ;


//------------------------------------------------------------------------------
// CORE BUS
//------------------------------------------------------------------------------

// CPU I-Code 
wire    [31:0]  HADDRI;
wire    [1:0]   HTRANSI;
wire    [2:0]   HSIZEI;
wire    [2:0]   HBURSTI;
wire    [3:0]   HPROTI;
wire    [31:0]  HRDATAI;
wire            HREADYI;
wire    [1:0]   HRESPI;

// CPU D-Code 
wire    [31:0]  HADDRD;
wire    [1:0]   HTRANSD;
wire    [2:0]   HSIZED;
wire    [2:0]   HBURSTD;
wire    [3:0]   HPROTD;
wire    [31:0]  HWDATAD;
wire            HWRITED;
wire    [31:0]  HRDATAD;
wire            HREADYD;
wire    [1:0]   HRESPD;
wire    [1:0]   HMASTERD;

// CPU System bus 
wire    [31:0]  HADDRS;
wire    [1:0]   HTRANSS;
wire            HWRITES;
wire    [2:0]   HSIZES;
wire    [31:0]  HWDATAS;
wire    [2:0]   HBURSTS;
wire    [3:0]   HPROTS;
wire            HREADYS;
wire    [31:0]  HRDATAS;
wire    [1:0]   HRESPS;
wire    [1:0]   HMASTERS;
wire            HMASTERLOCKS;


//------------------------------------------------------------------------------
// Instantiate Cortex-M3 processor 
//------------------------------------------------------------------------------

cortexm3ds_logic ulogic(
    // PMU
    .ISOLATEn                           (1'b1),
    .RETAINn                            (1'b1),

    // RESETS
    .PORESETn                           (RSTn),
    .SYSRESETn                          (cpuresetn),
    .SYSRESETREQ                        (SYSRESETREQ),
    .RSTBYPASS                          (1'b0),
    .CGBYPASS                           (1'b0),
    .SE                                 (1'b0),

    // CLOCKS
    .FCLK                               (clk),
    .HCLK                               (clk),
    .TRACECLKIN                         (1'b0),

    // SYSTICK
    .STCLK                              (1'b0),
    .STCALIB                            (26'b0),
    .AUXFAULT                           (32'b0),

    // CONFIG - SYSTEM
    .BIGEND                             (1'b0),
    .DNOTITRANS                         (1'b1),
    
    // SWJDAP
    .nTRST                              (1'b1),
    .SWDITMS                            (SWDI),
    .SWCLKTCK                           (swck),
    .TDI                                (1'b0),
    .CDBGPWRUPACK                       (CDBGPWRUPACK),
    .CDBGPWRUPREQ                       (CDBGPWRUPREQ),
    .SWDO                               (SWDO),
    .SWDOEN                             (SWDOEN),

    // IRQS
    .INTISR                             (IRQ),
    .INTNMI                             (1'b0),
    
    // I-CODE BUS
    .HREADYI                            (HREADYI),
    .HRDATAI                            (HRDATAI),
    .HRESPI                             (HRESPI),
    .IFLUSH                             (1'b0),
    .HADDRI                             (HADDRI),
    .HTRANSI                            (HTRANSI),
    .HSIZEI                             (HSIZEI),
    .HBURSTI                            (HBURSTI),
    .HPROTI                             (HPROTI),

    // D-CODE BUS
    .HREADYD                            (HREADYD),
    .HRDATAD                            (HRDATAD),
    .HRESPD                             (HRESPD),
    .EXRESPD                            (1'b0),
    .HADDRD                             (HADDRD),
    .HTRANSD                            (HTRANSD),
    .HSIZED                             (HSIZED),
    .HBURSTD                            (HBURSTD),
    .HPROTD                             (HPROTD),
    .HWDATAD                            (HWDATAD),
    .HWRITED                            (HWRITED),
    .HMASTERD                           (HMASTERD),

    // SYSTEM BUS
    .HREADYS                            (HREADYS),
    .HRDATAS                            (HRDATAS),
    .HRESPS                             (HRESPS),
    .EXRESPS                            (1'b0),
    .HADDRS                             (HADDRS),
    .HTRANSS                            (HTRANSS),
    .HSIZES                             (HSIZES),
    .HBURSTS                            (HBURSTS),
    .HPROTS                             (HPROTS),
    .HWDATAS                            (HWDATAS),
    .HWRITES                            (HWRITES),
    .HMASTERS                           (HMASTERS),
    .HMASTLOCKS                         (HMASTERLOCKS),

    // SLEEP
    .RXEV                               (1'b0),
    .SLEEPHOLDREQn                      (1'b1),
    .SLEEPING                           (SLEEPing),
    
    // EXTERNAL DEBUG REQUEST
    .EDBGRQ                             (1'b0),
    .DBGRESTART                         (1'b0),
    
    // DAP HMASTER OVERRIDE
    .FIXMASTERTYPE                      (1'b0),

    // WIC
    .WICENREQ                           (1'b0),

    // TIMESTAMP INTERFACE
    .TSVALUEB                           (48'b0),

    // CONFIG - DEBUG
    .DBGEN                              (1'b1),
    .NIDEN                              (1'b1),
    .MPUDISABLE                         (1'b0)
);


//------------------------------------------------------------------------------
// AHB BUS MATRIX
//------------------------------------------------------------------------------

// AHB ITCM
wire    [31:0]  HADDR_AHBP0;
wire    [1:0]   HTRANS_AHBP0;
wire            HWRITE_AHBP0;
wire    [2:0]   HSIZE_AHBP0;
wire    [31:0]  HWDATA_AHBP0;
wire    [2:0]   HBURST_AHBP0;
wire    [3:0]   HPROT_AHBP0;
wire            HREADY_AHBP0;
wire    [31:0]  HRDATA_AHBP0;
wire    [1:0]   HRESP_AHBP0;
wire            HREADYOUT_AHBP0;
wire            HSEL_AHBP0;
wire    [1:0]   HMASTER_AHBP0;
wire            HMASTERLOCK_AHBP0;

// AHB DTCM
wire    [31:0]  HADDR_AHBP1;
wire    [1:0]   HTRANS_AHBP1;
wire            HWRITE_AHBP1;
wire    [2:0]   HSIZE_AHBP1;
wire    [31:0]  HWDATA_AHBP1;
wire    [2:0]   HBURST_AHBP1;
wire    [3:0]   HPROT_AHBP1;
wire            HREADY_AHBP1;
wire    [31:0]  HRDATA_AHBP1;
wire    [1:0]   HRESP_AHBP1;
wire            HREADYOUT_AHBP1;
wire            HSEL_AHBP1;
wire    [1:0]   HMASTER_AHBP1;
wire            HMASTERLOCK_AHBP1;

// APB Bridge
wire    [31:0]  HADDR_AHBP2;
wire    [1:0]   HTRANS_AHBP2;
wire            HWRITE_AHBP2;
wire    [2:0]   HSIZE_AHBP2;
wire    [31:0]  HWDATA_AHBP2;
wire    [2:0]   HBURST_AHBP2;
wire    [3:0]   HPROT_AHBP2;
wire            HREADY_AHBP2;
wire    [31:0]  HRDATA_AHBP2;
wire    [1:0]   HRESP_AHBP2;
wire            HREADYOUT_AHBP2;
wire            HSEL_AHBP2;
wire    [1:0]   HMASTER_AHBP2;
wire            HMASTERLOCK_AHBP2;

AhbMtx    AhbMtx(
    .HCLK                               (clk),
    .HRESETn                            (cpuresetn),

    .REMAP                              (4'b0),

	 // I-CODE BUS
    .HSELS0                             (1'b1),
    .HADDRS0                            (HADDRI),
    .HTRANSS0                           (HTRANSI),
    .HWRITES0                           (1'b0),
    .HSIZES0                            (HSIZEI),
    .HBURSTS0                           (HBURSTI),
    .HPROTS0                            (HPROTI),
    .HMASTERS0                          (4'b0),
    .HWDATAS0                           (32'b0),
    .HMASTLOCKS0                        (1'b0),
    .HREADYS0                           (HREADYI),
    .HAUSERS0                           (32'b0),
    .HWUSERS0                           (32'b0),
    .HRDATAS0                           (HRDATAI),
    .HREADYOUTS0                        (HREADYI),
    .HRESPS0                            (HRESPI),
    .HRUSERS0                           (),

	 // D-CODE BUS
    .HSELS1                             (1'b1),
    .HADDRS1                            (HADDRD),
    .HTRANSS1                           (HTRANSD),
    .HWRITES1                           (HWRITED),
    .HSIZES1                            (HSIZED),
    .HBURSTS1                           (HBURSTD),
    .HPROTS1                            (HPROTD),
    .HMASTERS1                          ({2'b0,HMASTERD}),
    .HWDATAS1                           (HWDATAD),
    .HMASTLOCKS1                        (1'b0),
    .HREADYS1                           (HREADYD),
    .HAUSERS1                           (32'b0),
    .HWUSERS1                           (32'b0),
    .HRDATAS1                           (HRDATAD),
    .HREADYOUTS1                        (HREADYD),
    .HRESPS1                            (HRESPD),
    .HRUSERS1                           (),

	 // SYSTEM BUS
    .HSELS2                             (1'b1),
    .HADDRS2                            (HADDRS),
    .HTRANSS2                           (HTRANSS),
    .HWRITES2                           (HWRITES),
    .HSIZES2                            (HSIZES),
    .HBURSTS2                           (HBURSTS),
    .HPROTS2                            (HPROTS),
    .HMASTERS2                          ({2'b0,HMASTERS}),
    .HWDATAS2                           (HWDATAS),
    .HMASTLOCKS2                        (HMASTERLOCKS),
    .HREADYS2                           (HREADYS),
    .HAUSERS2                           (32'b0),
    .HWUSERS2                           (32'b0),
    .HRDATAS2                           (HRDATAS),
    .HREADYOUTS2                        (HREADYS),
    .HRESPS2                            (HRESPS),
    .HRUSERS2                           (),

	 // AHB ITCM
    .HSELM0                             (HSEL_AHBP0),
    .HADDRM0                            (HADDR_AHBP0),
    .HTRANSM0                           (HTRANS_AHBP0),
    .HWRITEM0                           (HWRITE_AHBP0),
    .HSIZEM0                            (HSIZE_AHBP0),
    .HBURSTM0                           (HBURST_AHBP0),
    .HPROTM0                            (HPROT_AHBP0),
    .HMASTERM0                          (HMASTER_AHBP0),
    .HWDATAM0                           (HWDATA_AHBP0),
    .HMASTLOCKM0                        (HMASTERLOCK_AHBP0),
    .HREADYMUXM0                        (HREADY_AHBP0),
    .HAUSERM0                           (),
    .HWUSERM0                           (),
    .HRDATAM0                           (HRDATA_AHBP0),
    .HREADYOUTM0                        (HREADYOUT_AHBP0),
    .HRESPM0                            (HRESP_AHBP0),
    .HRUSERM0                           (32'b0),

	 // AHB DTCM
    .HSELM1                             (HSEL_AHBP1),
    .HADDRM1                            (HADDR_AHBP1),
    .HTRANSM1                           (HTRANS_AHBP1),
    .HWRITEM1                           (HWRITE_AHBP1),
    .HSIZEM1                            (HSIZE_AHBP1),
    .HBURSTM1                           (HBURST_AHBP1),
    .HPROTM1                            (HPROT_AHBP1),
    .HMASTERM1                          (HMASTER_AHBP1),
    .HWDATAM1                           (HWDATA_AHBP1),
    .HMASTLOCKM1                        (HMASTERLOCK_AHBP1),
    .HREADYMUXM1                        (HREADY_AHBP1),
    .HAUSERM1                           (),
    .HWUSERM1                           (),
    .HRDATAM1                           (HRDATA_AHBP1),
    .HREADYOUTM1                        (HREADYOUT_AHBP1),
    .HRESPM1                            (HRESP_AHBP1),
    .HRUSERM1                           (32'b0),

	 // APB Bridge
    .HSELM2                             (HSEL_AHBP2),
    .HADDRM2                            (HADDR_AHBP2),
    .HTRANSM2                           (HTRANS_AHBP2),
    .HWRITEM2                           (HWRITE_AHBP2),
    .HSIZEM2                            (HSIZE_AHBP2),
    .HBURSTM2                           (HBURST_AHBP2),
    .HPROTM2                            (HPROT_AHBP2),
    .HMASTERM2                          (HMASTER_AHBP2),
    .HWDATAM2                           (HWDATA_AHBP2),
    .HMASTLOCKM2                        (HMASTERLOCK_AHBP2),
    .HREADYMUXM2                        (HREADY_AHBP2),
    .HAUSERM2                           (),
    .HWUSERM2                           (),
    .HRDATAM2                           (HRDATA_AHBP2),
    .HREADYOUTM2                        (HREADYOUT_AHBP2),
    .HRESPM2                            (HRESP_AHBP2),
    .HRUSERM2                           (32'b0),

    .SCANENABLE                         (1'b0),
    .SCANINHCLK                         (1'b0),
    .SCANOUTHCLK                        ()
);


//------------------------------------------------------------------------------
// AHB ITCM
//------------------------------------------------------------------------------

wire    [13:0]  ITCMADDR;
wire    [31:0]  ITCMRDATA,ITCMWDATA;
wire    [3:0]   ITCMWRITE;
wire            ITCMCS;

cmsdk_ahb_to_sram #(
    .AW                                 (16)
)   AhbItcm (
    .HCLK                               (clk),
    .HRESETn                            (cpuresetn),
    .HSEL                               (HSEL_AHBP0),
    .HREADY                             (HREADY_AHBP0),
    .HTRANS                             (HTRANS_AHBP0),
    .HSIZE                              (HSIZE_AHBP0),
    .HWRITE                             (HWRITE_AHBP0),
    .HADDR                              (HADDR_AHBP0),
    .HWDATA                             (HWDATA_AHBP0),
    .HREADYOUT                          (HREADYOUT_AHBP0),
    .HRESP                              (HRESP_AHBP0[0]),
    .HRDATA                             (HRDATA_AHBP0),
    .SRAMRDATA                          (ITCMRDATA),
    .SRAMADDR                           (ITCMADDR),
    .SRAMWEN                            (ITCMWRITE),
    .SRAMWDATA                          (ITCMWDATA),
    .SRAMCS                             (ITCMCS)
);
assign  HRESP_AHBP0[1]    =   1'b0;

cmsdk_fpga_sram #(
    .AW                                 (14)
)   ITCM    (
    .CLK                                (clk),
    .ADDR                               (ITCMADDR),
    .WDATA                              (ITCMWDATA),
    .WREN                               (ITCMWRITE),
    .CS                                 (ITCMCS),
    .RDATA                              (ITCMRDATA)
);


//------------------------------------------------------------------------------
// AHB DTCM
//------------------------------------------------------------------------------

wire    [13:0]  DTCMADDR;
wire    [31:0]  DTCMRDATA,DTCMWDATA;
wire    [3:0]   DTCMWRITE;
wire            DTCMCS;

cmsdk_ahb_to_sram #(
    .AW                                 (16)
)   AhbDtcm (
    .HCLK                               (clk),
    .HRESETn                            (cpuresetn),
    .HSEL                               (HSEL_AHBP1),
    .HREADY                             (HREADY_AHBP1),
    .HTRANS                             (HTRANS_AHBP1),
    .HSIZE                              (HSIZE_AHBP1),
    .HWRITE                             (HWRITE_AHBP1),
    .HADDR                              (HADDR_AHBP1),
    .HWDATA                             (HWDATA_AHBP1),
    .HREADYOUT                          (HREADYOUT_AHBP1),
    .HRESP                              (HRESP_AHBP1[0]),
    .HRDATA                             (HRDATA_AHBP1),
    .SRAMRDATA                          (DTCMRDATA),
    .SRAMADDR                           (DTCMADDR),
    .SRAMWEN                            (DTCMWRITE),
    .SRAMWDATA                          (DTCMWDATA),
    .SRAMCS                             (DTCMCS)
);
assign  HRESP_AHBP1[1]    =   1'b0;

cmsdk_fpga_sram #(
    .AW                                 (14)
)   DTCM    (
    .CLK                                (clk),
    .ADDR                               (DTCMADDR),
    .WDATA                              (DTCMWDATA),
    .WREN                               (DTCMWRITE),
    .CS                                 (DTCMCS),
    .RDATA                              (DTCMRDATA)
);


//------------------------------------------------------------------------------
// AHB to APB Bridge
//------------------------------------------------------------------------------

wire    [15:0]  PADDR;    
wire            PENABLE;  
wire            PWRITE;   
wire    [3:0]   PSTRB;    
wire    [2:0]   PPROT;    
wire    [31:0]  PWDATA;   
wire            PSEL;     
wire            APBACTIVE;                  
wire    [31:0]  PRDATA;   
wire            PREADY;  
wire            PSLVERR; 

cmsdk_ahb_to_apb #(
    .ADDRWIDTH                          (16),
    .REGISTER_RDATA                     (1),
    .REGISTER_WDATA                     (1)
)    ApbBridge  (
    .HCLK                               (clk),
    .HRESETn                            (cpuresetn),
    .PCLKEN                             (1'b1),
    .HSEL                               (HSEL_AHBP2),
    .HADDR                              (HADDR_AHBP2),
    .HTRANS                             (HTRANS_AHBP2),
    .HSIZE                              (HSIZE_AHBP2),
    .HPROT                              (HPROT_AHBP2),
    .HWRITE                             (HWRITE_AHBP2),
    .HREADY                             (HREADY_AHBP2),
    .HWDATA                             (HWDATA_AHBP2),
    .HREADYOUT                          (HREADYOUT_AHBP2),
    .HRDATA                             (HRDATA_AHBP2),
    .HRESP                              (HRESP_AHBP2[0]),        
    .PADDR                              (PADDR),
    .PENABLE                            (PENABLE),
    .PWRITE                             (PWRITE),
    .PSTRB                              (PSTRB),
    .PPROT                              (PPROT),
    .PWDATA                             (PWDATA),
    .PSEL                               (PSEL),
    .APBACTIVE                          (APBACTIVE),
    .PRDATA                             (PRDATA),
    .PREADY                             (PREADY),
    .PSLVERR                            (PSLVERR)
);
assign  HRESP_AHBP2[1]    =   1'b0;


//------------------------------------------------------------------------------
// APB Slave MUX
//------------------------------------------------------------------------------

// UART
wire            PSEL_APBP0;
wire            PREADY_APBP0;
wire    [31:0]  PRDATA_APBP0;
wire            PSLVERR_APBP0;

// Timer0
wire            PSEL_APBP1;
wire            PREADY_APBP1;
wire    [31:0]  PRDATA_APBP1;
wire            PSLVERR_APBP1;

// Timer1
wire            PSEL_APBP2;
wire            PREADY_APBP2;
wire    [31:0]  PRDATA_APBP2;
wire            PSLVERR_APBP2;

// LCD
wire            PSEL_APBP3;
wire            PREADY_APBP3;
wire    [31:0]  PRDATA_APBP3;
wire            PSLVERR_APBP3;

// Key
wire            PSEL_APBP4;
wire            PREADY_APBP4;
wire    [31:0]  PRDATA_APBP4;
wire            PSLVERR_APBP4;

// LED
wire            PSEL_APBP5;
wire            PREADY_APBP5;
wire    [31:0]  PRDATA_APBP5;
wire            PSLVERR_APBP5;

// Buzzer
wire            PSEL_APBP6;
wire            PREADY_APBP6;
wire    [31:0]  PRDATA_APBP6;
wire            PSLVERR_APBP6;

cmsdk_apb_slave_mux #(
    .PORT0_ENABLE                       (1),
    .PORT1_ENABLE                       (1),
    .PORT2_ENABLE                       (1),
    .PORT3_ENABLE                       (1),
    .PORT4_ENABLE                       (1),
    .PORT5_ENABLE                       (1),
    .PORT6_ENABLE                       (1),
    .PORT7_ENABLE                       (0),
    .PORT8_ENABLE                       (0),
    .PORT9_ENABLE                       (0),
    .PORT10_ENABLE                      (0),
    .PORT11_ENABLE                      (0),
    .PORT12_ENABLE                      (0),
    .PORT13_ENABLE                      (0),
    .PORT14_ENABLE                      (0),
    .PORT15_ENABLE                      (0)
)   ApbSystem   (
    .DECODE4BIT                         (PADDR[15:12]),
    .PSEL                               (PSEL),

	 // UART
    .PSEL0                              (PSEL_APBP0),
    .PREADY0                            (PREADY_APBP0),
    .PRDATA0                            (PRDATA_APBP0),
    .PSLVERR0                           (PSLVERR_APBP0),
     
	 // Timer0
    .PSEL1                              (PSEL_APBP1),
    .PREADY1                            (PREADY_APBP1),
    .PRDATA1                            (PRDATA_APBP1),
    .PSLVERR1                           (PSLVERR_APBP1),

	 // Timer1
    .PSEL2                              (PSEL_APBP2),
    .PREADY2                            (PREADY_APBP2),
    .PRDATA2                            (PRDATA_APBP2),
    .PSLVERR2                           (PSLVERR_APBP2),

	 // LCD
    .PSEL3                              (PSEL_APBP3),
    .PREADY3                            (PREADY_APBP3),
    .PRDATA3                            (PRDATA_APBP3),
    .PSLVERR3                           (PSLVERR_APBP3),

	 // Key
    .PSEL4                              (PSEL_APBP4),
    .PREADY4                            (PREADY_APBP4),
    .PRDATA4                            (PRDATA_APBP4),
    .PSLVERR4                           (PSLVERR_APBP4),

	 // LED
    .PSEL5                              (PSEL_APBP5),
    .PREADY5                            (PREADY_APBP5),
    .PRDATA5                            (PRDATA_APBP5),
    .PSLVERR5                           (PSLVERR_APBP5),

	 // Buzzer
    .PSEL6                              (PSEL_APBP6),
    .PREADY6                            (PREADY_APBP6),
    .PRDATA6                            (PRDATA_APBP6),
    .PSLVERR6                           (PSLVERR_APBP6),

    .PSEL7                              (),
    .PREADY7                            (1'b1),
    .PRDATA7                            (32'b0),
    .PSLVERR7                           (1'b0),

    .PSEL8                              (),
    .PREADY8                            (1'b1),
    .PRDATA8                            (32'b0),
    .PSLVERR8                           (1'b0),

    .PSEL9                              (),
    .PREADY9                            (1'b1),
    .PRDATA9                            (32'b0),
    .PSLVERR9                           (1'b0),

    .PSEL10                             (),
    .PREADY10                           (1'b1),
    .PRDATA10                           (32'b0),
    .PSLVERR10                          (1'b0),

    .PSEL11                             (),
    .PREADY11                           (1'b1),
    .PRDATA11                           (32'b0),
    .PSLVERR11                          (1'b0),

    .PSEL12                             (),
    .PREADY12                           (1'b1),
    .PRDATA12                           (32'b0),
    .PSLVERR12                          (1'b0),
    
    .PSEL13                             (),
    .PREADY13                           (1'b1),
    .PRDATA13                           (32'b0),
    .PSLVERR13                          (1'b0),

    .PSEL14                             (),
    .PREADY14                           (1'b1),
    .PRDATA14                           (32'b0),
    .PSLVERR14                          (1'b0),

    .PSEL15                             (),
    .PREADY15                           (1'b1),
    .PRDATA15                           (32'b0),
    .PSLVERR15                          (1'b0),

    .PREADY                             (PREADY),
    .PRDATA                             (PRDATA),
    .PSLVERR                            (PSLVERR)

);


//------------------------------------------------------------------------------
// UART
//------------------------------------------------------------------------------

wire            TXINT;
wire            RXINT;
wire            TXOVRINT;
wire            RXOVRINT;
wire            UARTINT;

cmsdk_apb_uart UART(
    .PCLK                               (clk),
    .PCLKG                              (clk),
    .PRESETn                            (cpuresetn),
    .PSEL                               (PSEL_APBP0),
    .PADDR                              (PADDR[11:2]),
    .PENABLE                            (PENABLE), 
    .PWRITE                             (PWRITE),
    .PWDATA                             (PWDATA),
    .ECOREVNUM                          (4'b0),
    .PRDATA                             (PRDATA_APBP0),
    .PREADY                             (PREADY_APBP0),
    .PSLVERR                            (PSLVERR_APBP0),
    .RXD                                (RXD),
    .TXD                                (TXD),
    .TXEN                               (TXEN),
    .BAUDTICK                           (BAUDTICK),
    .TXINT                              (TXINT),
    .RXINT                              (RXINT),
    .TXOVRINT                           (TXOVRINT),
    .RXOVRINT                           (RXOVRINT),
    .UARTINT                            (UARTINT)
);


//------------------------------------------------------------------------------
// Timer0
//------------------------------------------------------------------------------

wire            TIMER0INT;

cmsdk_apb_timer Timer0(
    .PCLK                               (clk),
    .PCLKG                              (clk),
    .PRESETn                            (cpuresetn),
    .PSEL                               (PSEL_APBP1),
    .PADDR                              (PADDR[11:2]),
    .PENABLE                            (PENABLE), 
    .PWRITE                             (PWRITE),
    .PWDATA                             (PWDATA),
    .ECOREVNUM                          (4'b0),
    .PRDATA                             (PRDATA_APBP1),
    .PREADY                             (PREADY_APBP1),
    .PSLVERR                            (PSLVERR_APBP1),
    .EXTIN                              (1'b0),
    .TIMERINT                           (TIMER0INT)
);


//------------------------------------------------------------------------------
// Timer1
//------------------------------------------------------------------------------

wire            TIMER1INT;

cmsdk_apb_timer Timer1(
    .PCLK                               (clk),
    .PCLKG                              (clk),
    .PRESETn                            (cpuresetn),
    .PSEL                               (PSEL_APBP2),
    .PADDR                              (PADDR[11:2]),
    .PENABLE                            (PENABLE), 
    .PWRITE                             (PWRITE),
    .PWDATA                             (PWDATA),
    .ECOREVNUM                          (4'b0),
    .PRDATA                             (PRDATA_APBP2),
    .PREADY                             (PREADY_APBP2),
    .PSLVERR                            (PSLVERR_APBP2),
    .EXTIN                              (1'b0),
    .TIMERINT                           (TIMER1INT)
);


//------------------------------------------------------------------------------
// LCD
//------------------------------------------------------------------------------

custom_apb_lcd #(
    .ADDRWIDTH                          (12)
)   LCD(
    .PCLK                               (clk),
    .PRESETn                            (cpuresetn),
    .PSEL                               (PSEL_APBP3),
    .PADDR                              (PADDR),
    .PENABLE                            (PENABLE),
    .PWRITE                             (PWRITE),
    .PWDATA                             (PWDATA),
    .ECOREVNUM                          (4'b0),
    .PRDATA                             (PRDATA_APBP3),
    .PREADY                             (PREADY_APBP3),
    .PSLVERR                            (PSLVERR_APBP3),
    .LCD_CS                             (LCD_CS),
    .LCD_RS                             (LCD_RS),
    .LCD_WR                             (LCD_WR),
    .LCD_RD                             (LCD_RD),
    .LCD_RST                            (LCD_RST),
    .LCD_BL_CTR                         (LCD_BL_CTR),
    .LCD_DATA                           (LCD_DATA)
);


//------------------------------------------------------------------------------
// Key
//------------------------------------------------------------------------------

custom_apb_key #(
    .ADDRWIDTH                          (12)
)   Key   (
    .PCLK                               (clk),
    .PRESETn                            (cpuresetn),
    .PSEL                               (PSEL_APBP4),
    .PADDR                              (PADDR),
    .PENABLE                            (PENABLE),
    .PWRITE                             (PWRITE),
    .PWDATA                             (PWDATA),
    .ECOREVNUM                          (4'b0),
    .PRDATA                             (PRDATA_APBP4),
    .PREADY                             (PREADY_APBP4),
    .PSLVERR                            (PSLVERR_APBP4),
    .keyIn                              (keyIn)
);


//------------------------------------------------------------------------------
// LED
//------------------------------------------------------------------------------

custom_apb_led #(
    .ADDRWIDTH                          (12)
)   LED(
    .PCLK                               (clk),
    .PRESETn                            (cpuresetn),
    .PSEL                               (PSEL_APBP5),
    .PADDR                              (PADDR),
    .PENABLE                            (PENABLE),
    .PWRITE                             (PWRITE),
    .PWDATA                             (PWDATA),
    .ECOREVNUM                          (4'b0),
    .PRDATA                             (PRDATA_APBP5),
    .PREADY                             (PREADY_APBP5),
    .PSLVERR                            (PSLVERR_APBP5),
    .ledOut                             (ledOut)
);


//------------------------------------------------------------------------------
// Buzzer
//------------------------------------------------------------------------------

custom_apb_buzzer #(
    .ADDRWIDTH                          (12)
)   Buzzer(
    .PCLK                               (clk),
    .PRESETn                            (cpuresetn),
    .PSEL                               (PSEL_APBP6),
    .PADDR                              (PADDR),
    .PENABLE                            (PENABLE),
    .PWRITE                             (PWRITE),
    .PWDATA                             (PWDATA),
    .ECOREVNUM                          (4'b0),
    .PRDATA                             (PRDATA_APBP6),
    .PREADY                             (PREADY_APBP6),
    .PSLVERR                            (PSLVERR_APBP6),
    .buzzerOut                          (buzzerOut)
);


//------------------------------------------------------------------------------
// INTERRUPT 
//------------------------------------------------------------------------------

assign IRQ = {235'b0,TIMER1INT,TIMER0INT,TXOVRINT|RXOVRINT,TXINT,RXINT};


endmodule
