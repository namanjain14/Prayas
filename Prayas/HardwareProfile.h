/*********************************************************************
 *
 *	Hardware specific definitions for:
 *    - PIC32 USB Starter Kit
 *    - PIC32MX460F512L
 *    - Ethernet PICtail Plus (ENC28J60)
 *
 *********************************************************************
 * FileName:        HardwareProfile.h
 * Dependencies:    Compiler.h
 * Processor:       PIC32
 * Compiler:        Microchip C32 v1.11 or higher
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * Copyright (C) 2002-2010 Microchip Technology Inc.  All rights
 * reserved.
 *
 * Microchip licenses to you the right to use, modify, copy, and
 * distribute:
 * (i)  the Software when embedded on a Microchip microcontroller or
 *      digital signal controller product ("Device") which is
 *      integrated into Licensee's product; or
 * (ii) ONLY the Software driver source files ENC28J60.c, ENC28J60.h,
 *		ENCX24J600.c and ENCX24J600.h ported to a non-Microchip device
 *		used in conjunction with a Microchip ethernet controller for
 *		the sole purpose of interfacing with the ethernet controller.
 *
 * You should refer to the license agreement accompanying this
 * Software for additional information regarding your rights and
 * obligations.
 *
 * THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT
 * WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT
 * LIMITATION, ANY WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF
 * PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
 * BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE
 * THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER
 * SIMILAR COSTS, WHETHER ASSERTED ON THE BASIS OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE), BREACH OF WARRANTY, OR OTHERWISE.
 *
 *
 * Author               Date		Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Howard Schlunder		09/16/2010	Regenerated for specific boards
 ********************************************************************/
#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

#include "Compiler.h"

// Define a macro describing this hardware set up (used in other files)
#define PIC32_USB_DM320003_1

// Set configuration fuses (but only in MainDemo.c where THIS_IS_STACK_APPLICATION is defined)
#if defined(THIS_IS_STACK_APPLICATION)
	#pragma config FPLLODIV = DIV_1, FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF, FPBDIV = DIV_1, POSCMOD = XT, FNOSC = PRIPLL, CP = OFF

	#pragma config UPLLEN   = ON            // USB PLL Enabled
	#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
	#pragma config WDTPS    = PS131072           // Watchdog Timer Postscale
	#pragma config OSCIOFNC = OFF           // CLKO Enable
	#pragma config IESO     = OFF           // Internal/External Switch-over
	#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
	#pragma config BWP      = OFF//ON            // Boot Flash Write Protect
	#pragma config PWP      = OFF           // Program Flash Write Protect
	#pragma config DEBUG    = OFF           // Background Debugger Enable

#endif

// Clock frequency values
// These directly influence timed events using the Tick module.  They also are used for UART and SPI baud rate generation.
#define GetSystemClock()		(80000000ul)			// Hz
#define GetInstructionClock()	(GetSystemClock()/1)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Might need changing if using Doze modes.
#define GetPeripheralClock()	(GetSystemClock()/1)	// Normally GetSystemClock()/4 for PIC18, GetSystemClock()/2 for PIC24/dsPIC, and GetSystemClock()/1 for PIC32.  Divisor may be different if using a PIC32 since it's configurable.

#define LED4_TRIS			(TRISDbits.TRISD1)			// as per Scimatic PARIJAT
#define LED4_IO			(LATDbits.LATD1)				//(PORTDbits.RD1)
#define LED5_TRIS			(TRISDbits.TRISD2)			// as per Scimatic PARIJAT
#define LED5_IO			(LATDbits.LATD2)				//(PORTDbits.RD2)				//
#define LED6_TRIS			(TRISDbits.TRISD3)			// as per Scimatic PARIJAT
#define LED6_IO			(LATDbits.LATD3)				//(PORTDbits.RD3)				
#define LED7_TRIS			(TRISDbits.TRISD4)			// as per Scimatic PARIJAT
#define LED7_IO			(LATDbits.LATD4)				//(PORTDbits.RD4)			
#define LED_GET()			((unsigned char)LATD & 0x07)
#define LED_PUT(a)			do{LATD = (LATD & 0xFFF8) | ((a)&0x07);}while(0)

// Momentary push buttons
#define BUTTON0_TRIS		(TRISDbits.TRISD6)	// Ref SW1
#define BUTTON0_IO			(PORTDbits.RD6)
#define BUTTON1_TRIS		(TRISDbits.TRISD7)	// Ref SW2
#define BUTTON1_IO			(PORTDbits.RD7)
#define BUTTON2_TRIS		(TRISDbits.TRISD13)	// Ref SW3
#define BUTTON2_IO			(PORTDbits.RD13)
#define BUTTON3_TRIS		(TRISDbits.TRISD13)	// No BUTTON3 on this board
#define BUTTON3_IO			(1)

#define GSM_VBAT_EN_TRIS 	(TRISEbits.TRISE7)
#define GSM_VBAT_EN_IO 		(LATEbits.LATE7)


//For the MAX 485 trnsmission on PC side communication
	
#define TX485_ENABLE_TRIS		(TRISAbits.TRISA0)			//According to schematic
#define TX485_ENABLE_IO		(PORTAbits.RA0)				//transmission pin enable
	
#define RX485_ENABLE_TRIS		(TRISAbits.TRISA1)			//According to schematic
#define RX485_ENABLE_IO		(PORTAbits.RA1)				//transmission pin enable
	
	
#define RESET_PIN_TRIS		(TRISEbits.TRISE1)			//Reset pin
#define RESET_PIN_IO			(PORTEbits.RE1)
	
#define RESET_SWCH_TRIS	(TRISGbits.TRISG14)			//Reset Switch
#define RESET_SWCH_IO		(PORTGbits.RG14)
	
#define POWERKEY_TRIS		(TRISBbits.TRISB13)			//power key for GSM
#define O_POWERKEY			(PORTBbits.RB13)

#define O_EmergancyOff 		(PORTBbits.RB12)

//ADC pins configuration

#define ADC_CHANNEL_1		AD1PCFGbits.PCFG0 
#define ADC_CHANNEL_2		AD1PCFGbits.PCFG2 
#define ADC_CHANNEL_3		AD1PCFGbits.PCFG3 
#define ADC_CHANNEL_4		AD1PCFGbits.PCFG4 
#define ADC_CHANNEL_5		AD1PCFGbits.PCFG8 
#define ADC_CHANNEL_6		AD1PCFGbits.PCFG9 
#define ADC_CHANNEL_7		AD1PCFGbits.PCFG10 
#define ADC_CHANNEL_8		AD1PCFGbits.PCFG11 		// Disable digital input on AN2 (potentiometer)

#define BOOT_LED_TRIS		(TRISEbits.TRISE1)
#define BOOT_LED_IO			(LATEbits.LATE1)

#define ETH_LED_TRIS		(TRISEbits.TRISE3)
#define ETH_LED_IO			(LATEbits.LATE3)

#define GPRS_LED_TRIS		(TRISEbits.TRISE4)
#define GPRS_LED_IO			(LATEbits.LATE4)

#define MODBUS_LED_TRIS	(TRISEbits.TRISE5)
#define MODBUS_LED_IO		(LATEbits.LATE5)

#define HRTBIT_LED_TRIS	(TRISEbits.TRISE2)			
#define HRTBIT_LED_IO		(LATEbits.LATE2)


//#define HRTBIT_LED_TRIS	(TRISDbits.TRISD1)			//for mpc
//#define HRTBIT_LED_IO		(LATDbits.LATD1)


#define ANALOG_LED_TRIS	(TRISEbits.TRISE6)
#define ANALOG_LED_IO		(LATEbits.LATE6)

// UART configuration (not too important since we don't have a UART 
// connector attached normally, but needed to compile if the STACK_USE_UART 
// or STACK_USE_UART2TCP_BRIDGE features are enabled.
//#define UARTTX_TRIS			(TRISFbits.TRISF3)
//#define UARTRX_TRIS			(TRISFbits.TRISF2)


// Specify which SPI to use for the ENC28J60 or ENC624J600.  SPI1 is 
// the topmost slot with pin 1 on it.  SPI2 is the middle slot 
// starting on pin 33.
//#define ENC_IN_SPI1
#define ENC_IN_SPI2

// Note that SPI1 cannot be used when using the PIC32 USB Starter 
// Board or PIC32 USB Starter Kit II due to the USB peripheral pins 
// mapping on top of the ordinary SPI1 pinout.  
#if defined(ENC_IN_SPI1) && (defined(__32MX460F512L__) || defined(__32MX795F512L__))
	#undef ENC_IN_SPI1
	#define ENC_IN_SPI2
#endif


//// ENC28J60 I/O pins
//#if defined ENC_IN_SPI1
//	#define ENC_CS_TRIS			(TRISDbits.TRISD14)
//	#define ENC_CS_IO			(PORTDbits.RD14)
//	//#define ENC_RST_TRIS		(TRISDbits.TRISD15)	// Not connected by default.  It is okay to leave this pin completely unconnected, in which case this macro should simply be left undefined.
//	//#define ENC_RST_IO		(PORTDbits.RD15)
//
//	// SPI SCK, SDI, SDO pins are automatically controlled by the 
//	#define ENC_SPI_IF			(IFS0bits.SPI1RXIF)
//	#define ENC_SSPBUF			(SPI1BUF)
//	#define ENC_SPICON1			(SPI1CON)
//	#define ENC_SPICON1bits		(SPI1CONbits)
//	#define ENC_SPIBRG			(SPI1BRG)
//	#define ENC_SPISTATbits		(SPI1STATbits)
//#elif defined ENC_IN_SPI2
//	#define ENC_CS_TRIS			(TRISFbits.TRISF12)
//	#define ENC_CS_IO			(PORTFbits.RF12)
//	//#define ENC_RST_TRIS		(TRISFbits.TRISF13)	// Not connected by default.  It is okay to leave this pin completely unconnected, in which case this macro should simply be left undefined.
//	//#define ENC_RST_IO		(PORTFbits.RF13)
//
//	// SPI SCK, SDI, SDO pins are automatically controlled by the 
//	// PIC32 SPI module 
//	#define ENC_SPI_IF			(IFS1bits.SPI2RXIF)
//	#define ENC_SSPBUF			(SPI2BUF)
//	#define ENC_SPICON1			(SPI2CON)
//	#define ENC_SPISTATbits		(SPI2STATbits)
//	#define ENC_SPICON1bits		(SPI2CONbits)
//	#define ENC_SPIBRG			(SPI2BRG)
//#endif
//

//// ENC624J600 Interface Configuration
//// Comment out ENC100_INTERFACE_MODE if you don't have an ENC624J600 or 
//// ENC424J600.  Otherwise, choose the correct setting for the interface you 
//// are using.  Legal values are:
////  - Commented out: No ENC424J600/624J600 present or used.  All other 
////                   ENC100_* macros are ignored.
////  - 0: SPI mode using CS, SCK, SI, and SO pins
////  - 1: 8-bit demultiplexed PSP Mode 1 with RD and WR pins
////  - 2: 8-bit demultiplexed PSP Mode 2 with R/Wbar and EN pins
////  - 3: 16-bit demultiplexed PSP Mode 3 with RD, WRL, and WRH pins
////  - 4: 16-bit demultiplexed PSP Mode 4 with R/Wbar, B0SEL, and B1SEL pins
////  - 5: 8-bit multiplexed PSP Mode 5 with RD and WR pins
////  - 6: 8-bit multiplexed PSP Mode 6 with R/Wbar and EN pins
////  - 9: 16-bit multiplexed PSP Mode 9 with AL, RD, WRL, and WRH pins
////  - 10: 16-bit multiplexed PSP Mode 10 with AL, R/Wbar, B0SEL, and B1SEL 
////        pins
#define ENC100_INTERFACE_MODE			0
//
//// If using a parallel interface, direct RAM addressing can be used (if all 
//// addresses wires are connected), or a reduced number of pins can be used 
//// for indirect addressing.  If using an SPI interface or PSP Mode 9 or 10 
//// (multiplexed 16-bit modes), which require all address lines to always be 
//// connected, then this option is ignored. Comment out or uncomment this 
//// macro to match your hardware connections.
//#define ENC100_PSP_USE_INDIRECT_RAM_ADDRESSING
//
//// ENC424J600/624J600 parallel indirect address remapping macro function.
//// This section translates SFR and RAM addresses presented to the 
//// ReadMemory() and WriteMemory() APIs in ENCX24J600.c to the actual 
//// addresses that must be presented on the parallel interface.  This macro 
//// must be modified to match your hardware if you are using an indirect PSP 
//// addressing mode (ENC100_PSP_USE_INDIRECT_RAM_ADDRESSING is defined) and 
//// have some of your address lines tied off to Vdd.  If you are using the 
//// SPI interface, then this section can be ignored or deleted.
#if (ENC100_INTERFACE_MODE == 1) || (ENC100_INTERFACE_MODE == 2) || (ENC100_INTERFACE_MODE == 5) || (ENC100_INTERFACE_MODE == 6) // 8-bit PSP
	#define ENC100_TRANSLATE_TO_PIN_ADDR(a)		((((a)&0x0100)<<6) | ((a)&0x00FF))
#elif (ENC100_INTERFACE_MODE == 3) || (ENC100_INTERFACE_MODE == 4) // 16-bit PSP
	#define ENC100_TRANSLATE_TO_PIN_ADDR(a)		(a)
#endif
//
//// Auto-crossover pins on Fast 100Mbps Ethernet PICtail/PICtail Plus.  If 
//// your circuit doesn't have such a feature, delete these two defines.
//#define ENC100_MDIX_TRIS				(TRISBbits.TRISB3)			///PATHAK (done comment)
//#define ENC100_MDIX_IO					(LATBbits.LATB3)			
//
//// ENC624J600 I/O control and status pins
//// If a pin is not required for your selected ENC100_INTERFACE_MODE 
//// interface selection (ex: WRH/B1SEL for PSP modes 1, 2, 5, and 6), then 
//// you can ignore, delete, or put anything for the pin definition.  Also, 
//// the INT and POR pins are entirely optional.  If not connected, comment 
//// them out.
#define ENC100_INT_TRIS					(TRISEbits.TRISE9)		// INT signal is optional and currently unused in the Microchip TCP/IP Stack.  Leave this pin disconnected and comment out this pin definition if you don't want it.
#define ENC100_INT_IO					(PORTEbits.RE9)
#if (ENC100_INTERFACE_MODE >= 1)	// Parallel mode
	// PSP control signal pinout
	#define ENC100_CS_TRIS					(TRISAbits.TRISA5)	// CS is optional in PSP mode.  If you are not sharing the parallel bus with another device, tie CS to Vdd and comment out this pin definition.
	#define ENC100_CS_IO					(LATAbits.LATA5)
	#define ENC100_POR_TRIS					(TRISCbits.TRISC1)	// POR signal is optional.  If your application doesn't have a power disconnect feature, comment out this pin definition.
	#define ENC100_POR_IO					(LATCbits.LATC1)
	#define ENC100_SO_WR_B0SEL_EN_TRIS		(TRISDbits.TRISD4)
	#define ENC100_SO_WR_B0SEL_EN_IO		(LATDbits.LATD4)
	#define ENC100_SI_RD_RW_TRIS			(TRISDbits.TRISD5)
	#define ENC100_SI_RD_RW_IO				(LATDbits.LATD5)
	#define ENC100_SCK_AL_TRIS				(TRISBbits.TRISB15)
	#define ENC100_SCK_AL_IO				(LATBbits.LATB15)
#else
	// SPI pinout
	#if defined ENC_IN_SPI1
		#define ENC100_CS_TRIS					(TRISDbits.TRISD14)	// CS is mandatory when using the SPI interface
		#define ENC100_CS_IO					(LATDbits.LATD14)
		#define ENC100_POR_TRIS					(TRISDbits.TRISD15)	// POR signal is optional.  If your application doesn't have a power disconnect feature, comment out this pin definition.
		#define ENC100_POR_IO					(LATDbits.LATD15)
		#define ENC100_SO_WR_B0SEL_EN_TRIS		(TRISFbits.TRISF7)	// SO is ENCX24J600 Serial Out, which needs to connect to the PIC SDI pin for SPI mode
		#define ENC100_SO_WR_B0SEL_EN_IO		(PORTFbits.RF7)
		#define ENC100_SI_RD_RW_TRIS			(TRISFbits.TRISF8)	// SI is ENCX24J600 Serial In, which needs to connect to the PIC SDO pin for SPI mode
		#define ENC100_SI_RD_RW_IO				(LATFbits.LATF8)
		#define ENC100_SCK_AL_TRIS				(TRISFbits.TRISF6)
	#elif defined ENC_IN_SPI2
		#define ENC100_CS_TRIS					(TRISFbits.TRISF12)
		#define ENC100_CS_IO					(LATFbits.LATF12)
		#define ENC100_POR_TRIS					(TRISFbits.TRISF13)
		#define ENC100_POR_IO					(LATFbits.LATF13)
		#define ENC100_SO_WR_B0SEL_EN_TRIS		(TRISGbits.TRISG7)	// NOTE: SO is ENC624J600 Serial Out, which needs to connect to the PIC SDI pin for SPI mode
		#define ENC100_SO_WR_B0SEL_EN_IO		(PORTGbits.RG7)
		#define ENC100_SI_RD_RW_TRIS			(TRISGbits.TRISG8)	// NOTE: SI is ENC624J600 Serial In, which needs to connect to the PIC SDO pin for SPI mode
		#define ENC100_SI_RD_RW_IO				(LATGbits.LATG8)
		#define ENC100_SCK_AL_TRIS				(TRISGbits.TRISG6)
		#define ENC100_SCK_AL_IO				(PORTGbits.RG6)		// NOTE: This must be the PORT, not the LATch like it is for the PSP interface.
	#endif
#endif
//
//
//// ENC624J600 SPI SFR register selection (controls which SPI peripheral to 
//// use on PICs with multiple SPI peripherals).  If a parallel interface is 
//// used (ENC100_INTERFACE_MODE is >= 1), then the SPI is not used and this 
//// section can be ignored or deleted.
#if defined ENC_IN_SPI1
	#define ENC100_ISR_ENABLE		(IEC0bits.INT2IE)
	#define ENC100_ISR_FLAG			(IFS0bits.INT2IF)
	#define ENC100_ISR_POLARITY		(INTCONbits.INT2EP)
	#define ENC100_ISR_PRIORITY		(IPC2bits.INT2IP)
	#define ENC100_SPI_ENABLE		(ENC100_SPICON1bits.ON)
	#define ENC100_SPI_IF			(IFS0bits.SPI1RXIF)
	#define ENC100_SSPBUF			(SPI1BUF)
	#define ENC100_SPICON1			(SPI1CON)
	#define ENC100_SPISTATbits		(SPI1STATbits)
	#define ENC100_SPICON1bits		(SPI1CONbits)
	#define ENC100_SPIBRG			(SPI1BRG)
#elif defined ENC_IN_SPI2
	#define ENC100_ISR_ENABLE		(IEC0bits.INT4IE)
	#define ENC100_ISR_FLAG			(IFS0bits.INT4IF)
	#define ENC100_ISR_POLARITY		(INTCONbits.INT4EP)
	#define ENC100_ISR_PRIORITY		(IPC2bits.INT4IP)
	#define ENC100_SPI_ENABLE		(ENC100_SPICON1bits.ON)
	#define ENC100_SPI_IF			(IFS1bits.SPI2RXIF)
	#define ENC100_SSPBUF			(SPI2BUF)
	#define ENC100_SPICON1			(SPI2CON)
	#define ENC100_SPISTATbits		(SPI2STATbits)
	#define ENC100_SPICON1bits		(SPI2CONbits)
	#define ENC100_SPIBRG			(SPI2BRG)
#endif
//

//////  Addeed For MDD Stack..........

	//	#define MEDIA_SOFT_DETECT   //shraddha
//        // Registers for the SPI module you want to use
//        #define MDD_USE_SPI_1
//        //#define MDD_USE_SPI_2
//
//		//SPI Configuration
//		#define SPI_START_CFG_1     (PRI_PRESCAL_64_1 | SEC_PRESCAL_8_1 | MASTER_ENABLE_ON | SPI_CKE_ON | SPI_SMP_ON)
//        #define SPI_START_CFG_2     (SPI_ENABLE)
//
//        // Define the SPI frequency
//        #define SPI_FREQUENCY			(8000000)
//    
//       
//            // Description: SD-SPI Chip Select Output bit
//        	#define SD_CS                LATBbits.LATB1
//        	// Description: SD-SPI Chip Select TRIS bit
//            #define SD_CS_TRIS          TRISBbits.TRISB1
//            
//            // Description: SD-SPI Card Detect Input bit
//            #define SD_CD               PORTFbits.RF0
//            // Description: SD-SPI Card Detect TRIS bit
//            #define SD_CD_TRIS          TRISFbits.TRISF0
//
//            // Description: SD-SPI Write Protect Check Input bit
//            #define SD_WE               PORTFbits.RF1
//            // Description: SD-SPI Write Protect Check TRIS bit
//            #define SD_WE_TRIS          TRISFbits.TRISF1
//                   
//            // Description: The main SPI control register
//            #define SPICON1             SPI1CON
//            // Description: The SPI status register
//            #define SPISTAT             SPI1STAT
//            // Description: The SPI Buffer
//            #define SPIBUF              SPI1BUF
//            // Description: The receive buffer full bit in the SPI status register
//            #define SPISTAT_RBF         SPI1STATbits.SPIRBF
//            // Description: The bitwise define for the SPI control register (i.e. _____bits)
//            #define SPICON1bits         SPI1CONbits
//            // Description: The bitwise define for the SPI status register (i.e. _____bits)
//            #define SPISTATbits         SPI1STATbits
//            // Description: The enable bit for the SPI module
//            #define SPIENABLE           SPICON1bits.ON
//            // Description: The definition for the SPI baud rate generator register (PIC32)
//            #define SPIBRG			    SPI1BRG
//
//            // Tris pins for SCK/SDI/SDO lines
//
//            // Description: The TRIS bit for the SCK pin
//            #define SPICLOCK            TRISDbits.TRISD10 //shraddha
//            // Description: The TRIS bit for the SDI pin
//            #define SPIIN               TRISCbits.TRISC4  //shraddha
//            // Description: The TRIS bit for the SDO pin
//            #define SPIOUT              TRISDbits.TRISD0  //shraddha
//            //SPI library functions
//            #define putcSPI             putcSPI1
//            #define getcSPI             getcSPI1
//            #define OpenSPI(config1, config2)   OpenSPI1(config1, config2)
//            
//			//for SD-SPI
//			#define USE_SD_INTERFACE_WITH_SPI

#endif // #ifndef HARDWARE_PROFILE_H

/******************************************************************************************


					 ADDED For USB Device


******************************************************************************************/


#define FOSC	80000000

// define the interface that the SD card is using i.e SPI 1
#define MEDIASD_IF_SPI1

// define the chip select pin function
#define _SetChipSelect(on)          {\
                                        mPORTBSetPinsDigitalOut(BIT_1);\
                                        if(on)\
                                            mPORTBClearBits(BIT_1);\
                                        else\
                                            mPORTBSetBits(BIT_1);\
                                    }

// Card detect - Read Only
#define SD_CD                   (mPORTFReadBits(BIT_0))             
#define SDSetCDDirection()      (mPORTFSetPinsDigitalIn(BIT_0))
// Write enable - Read Only
#define SD_WE                   (mPORTFReadBits(BIT_1))               
#define SDSetWEDirection()      (mPORTFSetPinsDigitalIn(BIT_1))

// SPI configuration defines
#define FATFS_SPI_START_CFG_1		(PRI_PRESCAL_64_1 | SEC_PRESCAL_8_1 | MASTER_ENABLE_ON | SPI_CKE_ON | SPI_SMP_ON)
#define FATFS_SPI_START_CFG_2		(SPI_ENABLE)



    /*******************************************************************/
    /******** USB stack hardware selection options *********************/
    /*******************************************************************/
    //This section is the set of definitions required by the MCHPFSUSB
    //  framework.  These definitions tell the firmware what mode it is
    //  running in, and where it can find the results to some information
    //  that the stack needs.
    //These definitions are required by every application developed with
    //  this revision of the MCHPFSUSB framework.  Please review each
    //  option carefully and determine which options are desired/required
    //  for your application.

    //#define USE_SELF_POWER_SENSE_IO
//    #define tris_self_power     TRISAbits.TRISA2    // Input
//    #define self_power          1

    //#define USE_USB_BUS_SENSE_IO
//    #define tris_usb_bus_sense  TRISBbits.TRISB5    // Input
//    #define USB_BUS_SENSE       1

    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/
    /******** Application specific definitions *************************/
    /*******************************************************************/
    /*******************************************************************/
    /*******************************************************************/

//    #define GetSystemClock()            80000000UL
//    #define GetPeripheralClock()        (GetSystemClock())
//    #define GetInstructionClock()       (GetSystemClock())

    /** LED ************************************************************/
//    #define mInitAllLEDs()      LATE = 0xFFF0; TRISE = 0xFFF0;
//
//    #define mLED_1              LATEbits.LATE0
//    #define mGetLED_1()         mLED_1
//    #define mLED_1_On()         mLED_1 = 0;
//    #define mLED_1_Off()        mLED_1 = 1;
//    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
//
//    /** SWITCH *********************************************************/
//    #define mInitSwitch2()      TRISEbits.TRISE7=1;
//    #define mInitAllSwitches()  mInitSwitch2()
//    #define sw2                 PORTEbits.RE7

