
#include <xc.h>


/******************************************************************************
 * Flash functions
 */

void Unlock_FLASH(void);
/*
 SUBROUTINE
 Start of required sequence to initiate erase and write cycles.
 Writes 55h, then AAh, to PMCON2 (Flash programming unlock sequence).
 Sets control bit WR of the PMCON1 register to begin the erase operation.
 Waits for WR to clear.
 Clears WREN to inhibit programming/erasing of Flash program memory
 *
 */

void Write_FLASH(void);
/*
 SUBROUTINE
 LoadS program memory write latches.
 Write of program memory write latches to program memory
 *
 */

#if defined(_10F320)|(_10LF320)
void Read_FLASH(uint8_t address);
/*
 SUBROUTINE
 Reads Flash program memory location and places data in PMDATL & PMDATH
 *
 */


uint16_t ReadWrite_HEFlash(uint8_t rw, uint16_t data, uint8_t rowstartaddr);
/*
 MAIN-ROUTINE
 Writes 12 bits of user data to the next available/open Flash program memory location (bits <11-0>) in specified page.
 Reads 12-bit user data of last location written. 

 *
 */
#endif

#if defined(_10F322)|(_10LF322)|(_12LF1552)|(_12F1501)|(_12F1572)
void Read_FLASH(uint16_t address);
/*
 See void read_FLASH(uint8_t address);
 *
 */
uint16_t ReadWrite_HEFlash(uint8_t rw, uint8_t data, uint16_t rowstartaddr);
/*
 See uint8_t readwrite_HEFlash(uint8_t data, uint8_t rowstartaddr);
 *
 */
#endif

void Save_INTERRUPT(void);
/*
 Save Interrupts
 *
 */




