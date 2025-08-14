/*
 * File:   PIC10_PIC12_HEFlash.c
 * The generated drivers are tested against the following:
 * Compiler          :  XC8 v1.21
 * MPLAB             :  MPLAB X 2.20
 *
 * Copyright (c) 2011-2012 Microchip Technology Inc. All rights reserved
 * Author: Willem J Smit
 *
 * Created on September 13, 2011
 *
 * MICROCHIP SOFTWARE NOTICE AND DISCLAIMER:  You may use this software, and any derivatives created
 * by any person or entity by or on your behalf, exclusively with Microchip?s products.  Microchip
 * and its licensors retain all ownership and intellectual property rights in the accompanying software
 * and in all derivatives hereto.
 *
 * This software and any accompanying information is for suggestion only.  It does not modify
 * Microchip?s standard warranty for its products.  You agree that you are solely responsible for
 * testing the software and determining its suitability.  Microchip has no obligation to modify,
 * test, certify, or support the software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS
 * FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE, ITS INTERACTION WITH MICROCHIP?S PRODUCTS, COMBINATION
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * IN NO EVENT, WILL MICROCHIP BE LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR
 * BREACH OF STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHErwISE, FOR ANY INDIRECT,
 * SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL
 * LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE TERMS.
 *
 */
/*
 Supported devices:
 * PIC10F320
 * PIC10LF320
 * PIC10F322
 * PIC10LF322
 * PIC12F1501
 * PIC12LF1552
 *PIC12LF1572
*/



#include <stdint.h>
#include "PIC10_PIC12_HEFlash.h"

uint8_t SaveInt;



/**
  Prototype        void  void Read_FLASH(uint8_t address)
  Param            rw: 1=Write, 0=Read
                   data: 12-bit
                   address
  Return           12-bit data
  Comment          See application note.
  Example usage    buffer = ReadWrite_HEFlash(0, 0x00, 0xF0); (Read example)
                   ReadWrite_HEFlash(1, buffer, 0xF0); (Write example)
*/



#if defined(_10F320)|(_10LF320)

    uint16_t ReadWrite_HEFlash(uint8_t rw, uint16_t data, uint8_t rowstartaddr) {
        uint8_t addr;
#endif

#if defined(_10F322)|(_10LF322)|(_12LF1552)|(_12F1501)|(_12F1572)

 /*----------------------------------------------------------------------------------------------------
 * NAME        : ReadWrite_HEFlash
 * DESCRIPTION : Reads from or writes data to the High Endurance Flash (HEF) memory based on 'rw' flag.
 *               If rw = 1, it writes data to HEF.
 *               If rw = 0, it reads the last written data from HEF.
 * PARAMS      : rw            - 1 for Write, 0 for Read
 *               data          - 8-bit data to write (or dummy for read)
 *               rowstartaddr  - Starting address of the flash row
 * RETURNS     : Data read from HEF (in read mode) or the data written (in write mode)
 *----------------------------------------------------------------------------------------------------*/       
        uint16_t ReadWrite_HEFlash(uint8_t rw, uint8_t data, uint16_t rowstartaddr) {
            uint16_t addr;
#endif

            //save_INTERRUPT();

            if (rw == 1) {
                addr = rowstartaddr;
            } else {
                addr = (rowstartaddr + 0xF);
            }
            uint8_t cnt;

            //CHECK FOR NEXT AVAILABLE WORD LOCATION
            for (cnt = 0x10; cnt != 0x0; cnt--) {
                Read_FLASH(addr); //Read FLASH

                if (rw == 1) {//SEARCH FOR NEXT OPEN LOCATION
                    if (PMDATH == 0x3F) { //IF NEXT ROW IS AVAILABLE
                        PMDAT = data;//PLACE USER DATA IN PMDAT TO BE WRITTEN TO FLASH
                        asm("bsf PMDATH, 5"); //SET FPM FREE FLAG (FPM LOCATION HAS BEEN WRITTEN)
                        asm("bcf PMDATH, 4");
                        Write_FLASH(); //Write TO FLASH
                        break;
                    }
                    addr++;

                    //CHECK IF THE ROW HAS EXPIRED
                    if (cnt <= 0x1) {
                        //THE ROW HAS EXPIRED IT WILL NOW BE ERASED
                        PMCON1 = 0b00010100; //CLEAR PM1CON & FREE = 1, WREN = 1
                        PMADR = rowstartaddr;
                        Unlock_FLASH();
                        //Write USER DATA
                        PMDAT = data; //PLACE USER DATA IN PMDAT TO BE WRITTEN TO FLASH
                        asm("bsf PMDATH, 5");//SET FPM FREE FLAG (FPM LOCATION HAS BEEN WRITTEN)
                        asm("bcf PMDATH, 4");
                        Write_FLASH(); //Write TO FLASH
                    }
                } else { //SEARCH FOR LAST WRITTEN LOCATION
                    if ((PMDATH & 0x20) && (PMDATH != 0x3F)) {//CHECK FPM FREE FLAG
                        data = (PMDAT & 0xFFF); //RETURN DATA FROM LAST WRITTEN LOCATION
                        break;
                    }
                    addr--;
                }
                //INTCON = SaveInt; //UNCOMMENT IF INTERRUPTS ARE USED.
            }
            return data ;
        }

/*----------------------------------------------------------------------------------------------------
 * NAME        : Save_INTERRUPT
 * DESCRIPTION : Saves the current interrupt enable status in global variable and disables global interrupt.
 * RETURNS     : None
 *----------------------------------------------------------------------------------------------------*/
        void Save_INTERRUPT(void) {
    SaveInt = INTCON; //save interrupt status
    GIE = 0; //Disable ints so required sequences will execute properly
}


#if defined(_10F320)|(_10LF320)

void Read_FLASH(uint8_t address) {
#endif

#if defined(_10F322)|(_10LF322)|(_12LF1552)|(_12F1501)|(_12F1572)

    /*----------------------------------------------------------------------------------------------------
 * NAME        : Read_FLASH
 * DESCRIPTION : Reads a single word from Flash memory at a given address.
 * PARAMS      : address - 8-bit or 16-bit address (depending on device)
 * RETURNS     : None
 *----------------------------------------------------------------------------------------------------*/
    void Read_FLASH(uint16_t address) {
#endif
        PMADR = address;
        PMCON1 = 0b00000001; //RD = 1 Read PROGRAM FLASH
        while (RD);
    }

/*----------------------------------------------------------------------------------------------------
 * NAME        : Unlock_FLASH
 * DESCRIPTION : Performs unlock sequence for flash write or erase operation.
 *               This allows programming or erasing flash.
 * RETURNS     : None
 *----------------------------------------------------------------------------------------------------*/    
void Unlock_FLASH(void) {
    PMCON2 = 0x55; //FLASH Write/ERASE Unlock SEQUENCE
    PMCON2 = 0xAA;
    WR = 1;
    while (WR);
    WREN = 0; //INHIBITS PROGRAMMING AND ERASING OF PROGRAM FLASH
}

/*----------------------------------------------------------------------------------------------------
 * NAME        : Write_FLASH
 * DESCRIPTION : Writes the data in PMDAT to flash memory using previously unlocked address.
 * RETURNS     : None
 *----------------------------------------------------------------------------------------------------*/
void Write_FLASH(void) {
    //PMCON1 = 0b00100100; //LOAD PROGRAM MEMORY Write LATCHES
    //Unlock_FLASH();
    PMCON1 = 0b00000100; //Write PROGRAM Write LATCHES TO PROGRAM MEMORY
    Unlock_FLASH();
}
