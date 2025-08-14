/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.4
        Device            :  PIC16F18313
        Driver Version    :  2.00
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

#include "mcc_generated_files/mcc.h"
#include "PIC10_PIC12_HEFlash.h"

// Macro definitions
#define LIGHT LATAbits.LATA1                                     // Define LIGHT as RA1 output pin (Relay or LED)                      

#define ON 1
#define OFF 0

#define passed 1
#define failed 0

// External flag set by interrupt when frequency pulse is received
extern bool Flag_FreCounter; 
uint32_t V_FreequencyCounter_u32 = 0;
static uint32_t V_Freequency_u32 = 0;

static uint32_t time_Scanning_Period = 0;
uint16_t time_Scanning_Period_count = 250; 

static uint32_t time_EEPROM_WRITE_Period = 0;
uint16_t time_EEPROM_WRITE_count = 10; 

#define MIN_RX_FREEQUENCY   15                // Minimum frequency
#define LIGHT_ADDR 0x0780                     //Starting Adress of Flash Memory

// Function declarations
bool get_DelayStatus(uint32_t *v_TimerStartingValue_u32r, uint16_t v_TimeDelay_u8r);

void Sensing_Receiver(void);
char Sensing_Validation(void);
void LIGHT_Trigger(unsigned char Status );
unsigned char old_light_status(void);


/*----------------------------------------------------------------------------------------------------
 * NAME        : main
 * DESCRIPTION : Main function initializes system and enters an infinite loop 
 *               to control a light based on sensor input.
 * PARAMETERS  : None
 * RETURNS     : int (never returns in embedded context)
 *----------------------------------------------------------------------------------------------------*/

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    
 
    bool V_Keypad_Long_Press = false;   
    bool V_LightStatus;
    V_LightStatus = old_light_status();
    
    while(1)
    {
         Sensing_Receiver();                                                  // frequency counter 
        if((Sensing_Validation() == passed) && (V_Keypad_Long_Press == true ))   //Check if validation passed 
          {
             if(V_LightStatus == OFF){                      //Check if previous Light status was OFF
                 V_LightStatus = ON;                        //Set LIGHT ON status
             }
             else{
                 V_LightStatus = OFF;                        //Set LIGHT OFF status
             }
             
            V_Keypad_Long_Press = false;                      //unset First_time_Active
            LIGHT_Trigger(V_LightStatus);                   //Trigger LIGHT ON/OFF
          }
         
        else if((Sensing_Validation() == failed)){          //Check for sensing validation
            V_Keypad_Long_Press=true;                         //Set First_Time_Active 
        }   
         
      if((get_DelayStatus(&time_EEPROM_WRITE_Period,time_EEPROM_WRITE_count)==true)) // delay for writing data  
      {
         ReadWrite_HEFlash(1,V_LightStatus,LIGHT_ADDR);            // store light status in EEPROM                                 
      }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------
 * NAME        : Sensing_Receiver
 * DESCRIPTION : Measures the input signal frequency by counting pulses using interrupt flag.
 *               Updates the frequency value every 250ms.
 *----------------------------------------------------------------------------------------------------*/
 void Sensing_Receiver(void)                                  //Frequency counter
{
    if(Flag_FreCounter == true)
        {
        if(V_FreequencyCounter_u32 == 0)                    
        {
            time_Scanning_Period = 0;
        }
            V_FreequencyCounter_u32++;
            Flag_FreCounter = false;
        }
    
     if((get_DelayStatus(&time_Scanning_Period,time_Scanning_Period_count)==true))  //Get frequency value every 150ms
        {
            V_Freequency_u32 = V_FreequencyCounter_u32;
            V_FreequencyCounter_u32 = 0;
        }
}

 // ///////////////////Validates if received frequency meets the minimum threshold//////////////////////////////
 /*----------------------------------------------------------------------------------------------------
 * NAME        : Sensing_Validation
 * DESCRIPTION : Validates whether the measured frequency is above the minimum threshold.
 * RETURNS     : 'passed' if valid frequency, 'failed' otherwise.
 *----------------------------------------------------------------------------------------------------*/
char Sensing_Validation(void)                                                   //validate Frequency count
{            
    if((V_Freequency_u32 >= MIN_RX_FREEQUENCY))
    {
        return passed;
    }
    else
    {
         return failed;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------
 * NAME        : LIGHT_Trigger
 * DESCRIPTION : Turns the light ON or OFF by setting the output pin.
 * PARAMETERS  : Status - ON or OFF
 *----------------------------------------------------------------------------------------------------*/
// Controls the LIGHT pin (Relay/LED)
void LIGHT_Trigger(unsigned char Status )                                      // Light TRigger ON/OFF
{   
    LIGHT=Status;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------------------------------------
 * NAME        : old_light_status
 * DESCRIPTION : Reads the last saved light status from EEPROM and applies it.
 * RETURNS     : Stored light status (ON/OFF)
 *----------------------------------------------------------------------------------------------------*/
// Reads previously stored light status from EEPROM and applies it
unsigned char old_light_status(void)
{
    unsigned char V_light_status_on = ReadWrite_HEFlash(0,0x00,LIGHT_ADDR);   // EEPROM read status store in light status on
    LIGHT_Trigger(V_light_status_on);                             
    return V_light_status_on;         
}

// End of File
