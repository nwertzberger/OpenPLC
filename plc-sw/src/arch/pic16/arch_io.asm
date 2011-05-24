;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.0.1 #6267 (Mar  7 2011) (Linux)
; This file was generated Wed Apr  6 14:14:36 2011
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f4550

	radix dec

;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global _arch_io_init
	global _arch_io_get_vin
	global _arch_io_set_vout
	global _arch_io_get_vout

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
	extern _SPPDATAbits
	extern _SPPCFGbits
	extern _SPPEPSbits
	extern _SPPCONbits
	extern _UFRMLbits
	extern _UFRMHbits
	extern _UIRbits
	extern _UIEbits
	extern _UEIRbits
	extern _UEIEbits
	extern _USTATbits
	extern _UCONbits
	extern _UADDRbits
	extern _UCFGbits
	extern _UEP0bits
	extern _UEP1bits
	extern _UEP2bits
	extern _UEP3bits
	extern _UEP4bits
	extern _UEP5bits
	extern _UEP6bits
	extern _UEP7bits
	extern _UEP8bits
	extern _UEP9bits
	extern _UEP10bits
	extern _UEP11bits
	extern _UEP12bits
	extern _UEP13bits
	extern _UEP14bits
	extern _UEP15bits
	extern _PORTAbits
	extern _PORTBbits
	extern _PORTCbits
	extern _PORTDbits
	extern _PORTEbits
	extern _LATAbits
	extern _LATBbits
	extern _LATCbits
	extern _LATDbits
	extern _LATEbits
	extern _TRISAbits
	extern _TRISBbits
	extern _TRISCbits
	extern _TRISDbits
	extern _TRISEbits
	extern _OSCTUNEbits
	extern _PIE1bits
	extern _PIR1bits
	extern _IPR1bits
	extern _PIE2bits
	extern _PIR2bits
	extern _IPR2bits
	extern _EECON1bits
	extern _RCSTAbits
	extern _TXSTAbits
	extern _T3CONbits
	extern _CMCONbits
	extern _CVRCONbits
	extern _ECCP1ASbits
	extern _ECCP1DELbits
	extern _BAUDCONbits
	extern _CCP2CONbits
	extern _CCP1CONbits
	extern _ADCON2bits
	extern _ADCON1bits
	extern _ADCON0bits
	extern _SSPCON2bits
	extern _SSPCON1bits
	extern _SSPSTATbits
	extern _T2CONbits
	extern _T1CONbits
	extern _RCONbits
	extern _WDTCONbits
	extern _HLVDCONbits
	extern _OSCCONbits
	extern _T0CONbits
	extern _STATUSbits
	extern _FSR2Hbits
	extern _BSRbits
	extern _FSR1Hbits
	extern _FSR0Hbits
	extern _INTCON3bits
	extern _INTCON2bits
	extern _INTCONbits
	extern _TBLPTRUbits
	extern _PCLATHbits
	extern _PCLATUbits
	extern _STKPTRbits
	extern _TOSUbits
	extern _SPPDATA
	extern _SPPCFG
	extern _SPPEPS
	extern _SPPCON
	extern _UFRML
	extern _UFRMH
	extern _UIR
	extern _UIE
	extern _UEIR
	extern _UEIE
	extern _USTAT
	extern _UCON
	extern _UADDR
	extern _UCFG
	extern _UEP0
	extern _UEP1
	extern _UEP2
	extern _UEP3
	extern _UEP4
	extern _UEP5
	extern _UEP6
	extern _UEP7
	extern _UEP8
	extern _UEP9
	extern _UEP10
	extern _UEP11
	extern _UEP12
	extern _UEP13
	extern _UEP14
	extern _UEP15
	extern _PORTA
	extern _PORTB
	extern _PORTC
	extern _PORTD
	extern _PORTE
	extern _LATA
	extern _LATB
	extern _LATC
	extern _LATD
	extern _LATE
	extern _TRISA
	extern _TRISB
	extern _TRISC
	extern _TRISD
	extern _TRISE
	extern _OSCTUNE
	extern _PIE1
	extern _PIR1
	extern _IPR1
	extern _PIE2
	extern _PIR2
	extern _IPR2
	extern _EECON1
	extern _EECON2
	extern _EEDATA
	extern _EEADR
	extern _RCSTA
	extern _TXSTA
	extern _TXREG
	extern _RCREG
	extern _SPBRG
	extern _SPBRGH
	extern _T3CON
	extern _TMR3L
	extern _TMR3H
	extern _CMCON
	extern _CVRCON
	extern _ECCP1AS
	extern _ECCP1DEL
	extern _BAUDCON
	extern _CCP2CON
	extern _CCPR2L
	extern _CCPR2H
	extern _CCP1CON
	extern _CCPR1L
	extern _CCPR1H
	extern _ADCON2
	extern _ADCON1
	extern _ADCON0
	extern _ADRESL
	extern _ADRESH
	extern _SSPCON2
	extern _SSPCON1
	extern _SSPSTAT
	extern _SSPADD
	extern _SSPBUF
	extern _T2CON
	extern _PR2
	extern _TMR2
	extern _T1CON
	extern _TMR1L
	extern _TMR1H
	extern _RCON
	extern _WDTCON
	extern _HLVDCON
	extern _OSCCON
	extern _T0CON
	extern _TMR0L
	extern _TMR0H
	extern _STATUS
	extern _FSR2L
	extern _FSR2H
	extern _PLUSW2
	extern _PREINC2
	extern _POSTDEC2
	extern _POSTINC2
	extern _INDF2
	extern _BSR
	extern _FSR1L
	extern _FSR1H
	extern _PLUSW1
	extern _PREINC1
	extern _POSTDEC1
	extern _POSTINC1
	extern _INDF1
	extern _WREG
	extern _FSR0L
	extern _FSR0H
	extern _PLUSW0
	extern _PREINC0
	extern _POSTDEC0
	extern _POSTINC0
	extern _INDF0
	extern _INTCON3
	extern _INTCON2
	extern _INTCON
	extern _PRODL
	extern _PRODH
	extern _TABLAT
	extern _TBLPTRL
	extern _TBLPTRH
	extern _TBLPTRU
	extern _PCL
	extern _PCLATH
	extern _PCLATU
	extern _STKPTR
	extern _TOSL
	extern _TOSH
	extern _TOSU
;--------------------------------------------------------
;	Equates to used internal registers
;--------------------------------------------------------
STATUS	equ	0xfd8
WREG	equ	0xfe8
FSR0L	equ	0xfe9
FSR0H	equ	0xfea
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
INDF0	equ	0xfef
POSTINC0	equ	0xfee
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PLUSW2	equ	0xfdb
PRODL	equ	0xff3


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1
r0x03	res	1
r0x04	res	1
r0x05	res	1

udata_arch_io_0	udata
_output_voltages	res	16

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; I code from now on!
; ; Starting pCode block
S_arch_io__arch_io_get_vout	code
_arch_io_get_vout:
;	.line	108; arch_io.c	uint16_t arch_io_get_vout(uint8_t port)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
; ;multiply lit val:0x02 by variable r0x00 and store in r0x00
; ;Unrolled 8 X 8 multiplication
; ;FIXME: the function does not support result==WREG
;	.line	111; arch_io.c	return output_voltages[port];
	BCF	STATUS, 0
	RLCF	r0x00, F
	CLRF	r0x01
	MOVLW	LOW(_output_voltages)
	ADDWF	r0x00, F
	MOVLW	HIGH(_output_voltages)
	ADDWFC	r0x01, F
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, FSR0H
	MOVFF	POSTINC0, r0x00
	MOVFF	INDF0, r0x01
	MOVFF	r0x01, PRODL
	MOVF	r0x00, W
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_arch_io__arch_io_set_vout	code
_arch_io_set_vout:
;	.line	88; arch_io.c	void arch_io_set_vout(uint8_t port, uint16_t voltage)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
; ;multiply lit val:0x02 by variable r0x00 and store in r0x03
; ;Unrolled 8 X 8 multiplication
; ;FIXME: the function does not support result==WREG
;	.line	93; arch_io.c	output_voltages[port] = voltage;
	BCF	STATUS, 0
	RLCF	r0x00, W
	MOVWF	r0x03
	CLRF	r0x04
	MOVLW	LOW(_output_voltages)
	ADDWF	r0x03, F
	MOVLW	HIGH(_output_voltages)
	ADDWFC	r0x04, F
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, FSR0H
	MOVFF	r0x01, POSTINC0
	MOVFF	r0x02, INDF0
;	.line	95; arch_io.c	if (voltage > 0)
	MOVF	r0x01, W
	IORWF	r0x02, W
	BZ	_00133_DS_
;	.line	97; arch_io.c	PORTD |= 1 << port;
	MOVLW	0x01
	MOVWF	r0x01
	MOVF	r0x00, W
	BZ	_00138_DS_
	NEGF	WREG
	BCF	STATUS, 0
_00139_DS_:
	RLCF	r0x01, F
	ADDLW	0x01
	BNC	_00139_DS_
_00138_DS_:
	MOVF	r0x01, W
	IORWF	_PORTD, F
	BRA	_00135_DS_
_00133_DS_:
;	.line	101; arch_io.c	PORTD &= ~(1 << port);
	MOVLW	0x01
	MOVWF	r0x01
	MOVF	r0x00, W
	BZ	_00141_DS_
	NEGF	WREG
	BCF	STATUS, 0
_00142_DS_:
	RLCF	r0x01, F
	ADDLW	0x01
	BNC	_00142_DS_
_00141_DS_:
	COMF	r0x01, W
	MOVWF	r0x00
	MOVF	r0x00, W
	ANDWF	_PORTD, F
_00135_DS_:
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_arch_io__arch_io_get_vin	code
_arch_io_get_vin:
;	.line	62; arch_io.c	uint16_t arch_io_get_vin(uint8_t port)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
;	.line	69; arch_io.c	ADCON0 = (port << 2) | 0x03;
	RLNCF	r0x00, W
	RLNCF	WREG, W
	ANDLW	0xfc
	MOVWF	r0x01
	MOVLW	0x03
	IORWF	r0x01, W
	MOVWF	_ADCON0
_00119_DS_:
;	.line	73; arch_io.c	while (ADCON0 & 0x02);
	BTFSC	_ADCON0, 1
	BRA	_00119_DS_
;	.line	74; arch_io.c	voltage = (ADRESH << 8) | ADRESL;
	MOVFF	_ADRESH, r0x00
	CLRF	r0x01
	MOVF	r0x00, W
	MOVWF	r0x03
	CLRF	r0x02
	MOVFF	_ADRESL, r0x00
	CLRF	r0x01
	MOVF	r0x00, W
	IORWF	r0x02, F
	MOVF	r0x01, W
	IORWF	r0x03, F
;	.line	79; arch_io.c	voltage = (voltage << 4) | (voltage << 3);
	SWAPF	r0x03, W
	ANDLW	0xf0
	MOVWF	r0x01
	SWAPF	r0x02, W
	MOVWF	r0x00
	ANDLW	0x0f
	XORWF	r0x00, F
	ADDWF	r0x01, F
	MOVF	r0x02, W
	MOVWF	r0x04
	ADDWF	r0x04, F
	RLCF	r0x03, W
	MOVWF	r0x05
	BCF	STATUS, 0
	RLCF	r0x04, F
	RLCF	r0x05, F
	BCF	STATUS, 0
	RLCF	r0x04, F
	RLCF	r0x05, F
	MOVF	r0x04, W
	IORWF	r0x00, W
	MOVWF	r0x02
	MOVF	r0x05, W
	IORWF	r0x01, W
	MOVWF	r0x03
;	.line	80; arch_io.c	return voltage;
	MOVFF	r0x03, PRODL
	MOVF	r0x02, W
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_arch_io__arch_io_init	code
_arch_io_init:
;	.line	24; arch_io.c	void arch_io_init()
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
;	.line	29; arch_io.c	TRISA = 0xff;   /* Ports A0-A3, A5  */
	MOVLW	0xff
	MOVWF	_TRISA
;	.line	30; arch_io.c	TRISE = 0xff;   /* Ports E0-E2      */
	MOVLW	0xff
	MOVWF	_TRISE
;	.line	38; arch_io.c	ADCON1 = 0x07;
	MOVLW	0x07
	MOVWF	_ADCON1
;	.line	47; arch_io.c	ADCON2 = 0x96;
	MOVLW	0x96
	MOVWF	_ADCON2
;	.line	50; arch_io.c	TRISD = 0x00;   /* Ports D0-D7  */
	CLRF	_TRISD
;	.line	51; arch_io.c	for(i=0; i<NUM_PORTS; i++)
	CLRF	r0x00
_00105_DS_:
	MOVLW	0x08
	SUBWF	r0x00, W
	BC	_00109_DS_
;	.line	53; arch_io.c	arch_io_set_vout(i, 0);
	MOVLW	0x00
	MOVWF	POSTDEC1
	MOVLW	0x00
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_arch_io_set_vout
	MOVLW	0x03
	ADDWF	FSR1L, F
;	.line	51; arch_io.c	for(i=0; i<NUM_PORTS; i++)
	INCF	r0x00, F
	BRA	_00105_DS_
_00109_DS_:
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	



; Statistics:
; code size:	  472 (0x01d8) bytes ( 0.36%)
;           	  236 (0x00ec) words
; udata size:	   16 (0x0010) bytes ( 0.89%)
; access size:	    6 (0x0006) bytes


	end
