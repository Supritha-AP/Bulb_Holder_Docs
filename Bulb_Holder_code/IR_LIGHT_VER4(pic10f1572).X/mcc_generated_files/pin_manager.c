/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.0
        Device            :  PIC12F1572
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.10 and above
        MPLAB             :  MPLAB X 5.35

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "pin_manager.h"
#include "mcc.h"

bool Flag_FreCounter = false;



void (*IOCAF0_InterruptHandler)(void);


void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATA = 0x00;

    /**
    TRISx registers
    */
    //TRISA = 0x2A;      old 
     TRISA = 0x09;    // new 0000 1001

    /**
    ANSELx registers
    */
    ANSELA = 0x02;//0000 0010

    /**
    WPUx registers
    */
    WPUA = 0x00;
    OPTION_REGbits.nWPUEN = 1;

    /**
    ODx registers
    */
    ODCONA = 0x00;

    /**
    SLRCONx registers
    */
    SLRCONA = 0x37;//0011 0111

    /**
    INLVLx registers
    */
    INLVLA = 0x3F;//0011 1111

    /**
    APFCONx registers
    */
    APFCON = 0x04;//0000 0100

    /**
    IOCx registers 
    */
//    //interrupt on change for group IOCAF - flag
//    IOCAFbits.IOCAF5 = 0;
//    //interrupt on change for group IOCAN - negative             //old
//    IOCANbits.IOCAN5 = 1;
//    //interrupt on change for group IOCAP - positive
//    IOCAPbits.IOCAP5 = 0;
    
     //interrupt on change for group IOCAF - flag
    IOCAFbits.IOCAF0 = 0;
    //interrupt on change for group IOCAN - negative
    IOCANbits.IOCAN0 = 1;
    //interrupt on change for group IOCAP - positive
    IOCAPbits.IOCAP0 = 0;



    // register default IOC callback functions at runtime; use these methods to register a custom function
    IOCAF0_SetInterruptHandler(IOCAF0_DefaultInterruptHandler);
   
    
    // Enable IOCI interrupt 
    INTCONbits.IOCIE = 1; 
    
}
  
void PIN_MANAGER_IOC(void)
{   
	// interrupt on change for pin IOCAF5
    if(IOCAFbits.IOCAF0 == 1)
    {
        IOCAF0_ISR();  
    }	
}

/**
   IOCAF5 Interrupt Service Routine
*/
void IOCAF0_ISR(void) {

    // Add custom IOCAF5 code
    Flag_FreCounter = true;
    // Call the interrupt handler for the callback registered at runtime
    if(IOCAF0_InterruptHandler)
    {
        IOCAF0_InterruptHandler();
    }
    IOCAFbits.IOCAF0 = 0;
}

/**
  Allows selecting an interrupt handler for IOCAF5 at application runtime
*/
void IOCAF0_SetInterruptHandler(void (* InterruptHandler)(void)){
    IOCAF0_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCAF5
*/
void IOCAF0_DefaultInterruptHandler(void){
    // add your IOCAF5 interrupt custom code
    // or set custom function using IOCAF5_SetInterruptHandler()
}

/**
 End of File
*/