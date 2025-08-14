/*
 * File:   SoftDelay.c
 * Author: Manoj Prasad
 *
 * Created on June 7, 2020, 9:33 PM
 */


#include <xc.h>

#include "SoftDelay.h"
/**
      
     @Summary
       This function is used as delay.

     @Description
       This function will be implemented to generate delay as much as user required.       

     @Preconditions
       Initialize *v_TimerStartingValue_u32r and *v_TimerCurrentValue_u32r as zero.

     @Param
       *v_TimerStartingValue_u32r  -> get the starting timer value.
       *v_TimerCurrentValue_u32r   -> Get the current timer status value.
       *v_TimeDelay_u16r           -> User to define any delay value im milliseconds .
  
     @Returns
     true  -  Difference between *v_TimerStartingValue_u32r and *v_TimerCurrentValue_u32r is equal to *v_TimeDelay_u16r then return TRUE
     false -  Difference between *v_TimerStartingValue_u32r and *v_TimerCurrentValue_u32r is not equal to *v_TimeDelay_u16r then return FALSE
     
     * 
     * 
     **/
/*----------------------------------------------------------------------------------------------------
 * NAME        : get_DelayStatus
 * DESCRIPTION : Checks if the required time delay has elapsed since the start time.
 * PARAMS      : v_TimerStartingValue_u32r - Pointer to the timer start value
 *               v_TimeDelay_u16r          - Required delay duration in milliseconds
 * RETURNS     : true  - If delay is completed
 *               false - If delay is not yet completed
 *----------------------------------------------------------------------------------------------------*/

    bool get_DelayStatus(uint32_t *v_TimerStartingValue_u32r, uint16_t v_TimeDelay_u16r)
{
    uint32_t v_TimeDifference_Value_u32r = get_Delay_Time(v_TimerStartingValue_u32r);
    
    if( v_TimeDifference_Value_u32r >= v_TimeDelay_u16r)
    {       
        *v_TimerStartingValue_u32r =0;
        return true;
    }
    else
    {
        return false;
    } 
}

    /**
     *
     @Summary
       This function is used as to calculate time difference value.

     @Description
       This function will calculate time difference value upto 2 power 32 milli seconds.
       Note: make value of *v_Timer_StartingValue_u32r and *v_Timer_CurrentValue_u32r as zero from where it will call.           

     @Preconditions
       Initialize *v_Timer_StartingValue_u32r and *v_Timer_CurrentValue_u32r as zero.

     @Param
       *v_Timer_StartingValue_u32r  -> get the starting timer value.
       *v_Timer_CurrentValue_u32r   -> Get the current timer status value.       
       v_Time_Diff_Value_u32r       -> It holds the difference value
  
     @Returns
       v_Time_Diff_Value_u32r -> It returns the difference value
     
     * 
     **/
/*----------------------------------------------------------------------------------------------------
 * NAME        : get_Delay_Time
 * DESCRIPTION : Calculates the elapsed time from the start timer value to the current timer value.
 * PARAMS      : v_Timer_StartingValue_u32r - Pointer to the starting timer value
 * RETURNS     : Elapsed time (difference) in milliseconds
 *----------------------------------------------------------------------------------------------------*/
uint32_t get_Delay_Time(uint32_t *v_Timer_StartingValue_u32r)
{
    uint32_t v_Time_Diff_Value_u32r,v_Timer_CurrentValue_u32r;
    
    //	timer values calibration is done here
    if(*v_Timer_StartingValue_u32r==0)
    {
       *v_Timer_StartingValue_u32r =get_TIMER0Status()-1;
       v_Timer_CurrentValue_u32r=get_TIMER0Status();
    }    
    else 
    {
        v_Timer_CurrentValue_u32r = get_TIMER0Status();
    }
    
// timer difference is calculated there
    if(*v_Timer_StartingValue_u32r > v_Timer_CurrentValue_u32r)
    {
        v_Time_Diff_Value_u32r = (4294967295 - *v_Timer_StartingValue_u32r) + v_Timer_CurrentValue_u32r ;        
    }
    else 
    {
        v_Time_Diff_Value_u32r = v_Timer_CurrentValue_u32r - *v_Timer_StartingValue_u32r;
    }    
    return v_Time_Diff_Value_u32r;    
}

/**
     *
     @Summary
       This function is used to initialize soft delay of Initial parameter as zero.

     @Preconditions
       parameter should be a pointer variable

     @Param
       *Start_vr_reset_u32r
  
     @Returns
       Nothing return
     
     * 
     **/


/**
     **********************************************************************
 Input: the input parameter int x
 * output : char buf
 * function name : Gte_Delay_Start_vr()
 * discription :
 * ************************************************************************* 
**/
/*----------------------------------------------------------------------------------------------------
 * NAME        : get_Delay_Start_vr
 * DESCRIPTION : Resets the delay timer by initializing the start time to 0.
 * PARAMS      : Start_vr_reset_u32r - Pointer to the variable to reset
 * RETURNS     : None
 *----------------------------------------------------------------------------------------------------*/

void get_Delay_Start_vr(uint32_t *Start_vr_reset_u32r)
{
    *Start_vr_reset_u32r = 0;//this line will do some work 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
