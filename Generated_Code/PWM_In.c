/* ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : PWM_In.c
**     Project     : TX9BVA
**     Processor   : MC9S08PA4VTG
**     Component   : Capture
**     Version     : Component 02.223, Driver 01.30, CPU db: 3.00.000
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2015-03-04, 22:13, # CodeGen: 2
**     Abstract    :
**         This component "Capture" simply implements the capture function
**         of timer. The counter counts the same way as in free run mode. On
**         the selected edge of the input signal (on the input pin), the current
**         content of the counter register is written into the capture
**         register and the OnCapture event is called.
**     Settings    :
**             Timer capture encapsulation : Capture
**
**         Timer
**             Timer                   : FTM0
**             Counter shared          : No
**
**         High speed mode
**             Prescaler               : divide-by-4
**           Maximal time for capture register
**             Xtal ticks              : 512
**             microseconds            : 16384
**             milliseconds            : 16
**             seconds (real)          : 0.016384
**             Hz                      : 61
**           One tick of timer is
**             nanoseconds             : 250
**
**         Initialization:
**              Timer                  : Enabled
**              Events                 : Enabled
**
**         Timer registers
**              Capture                : FTM0_C1V  [$0029]
**              Counter                : FTM0_CNT  [$0021]
**              Mode                   : FTM0_SC   [$0020]
**              Run                    : FTM0_SC   [$0020]
**              Prescaler              : FTM0_SC   [$0020]
**
**         Used input pin              : 
**             ----------------------------------------------------
**                Number (on package)  |    Name
**             ----------------------------------------------------
**                       15            |  PTA1_KBI0P1_FTM0CH1_ACMP1_ADP1
**             ----------------------------------------------------
**
**         Port name                   : PTA
**         Bit number (in port)        : 1
**         Bit mask of the port        : $0002
**
**         Signal edge/level           : both
**         Priority                    : 2
**         Pull option                 : off
**
**     Contents    :
**         Reset           - byte PWM_In_Reset(void);
**         GetCaptureValue - byte PWM_In_GetCaptureValue(PWM_In_TCapturedValue *Value);
**         GetPinValue     - bool PWM_In_GetPinValue(void);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file PWM_In.c
** @version 01.30
** @brief
**         This component "Capture" simply implements the capture function
**         of timer. The counter counts the same way as in free run mode. On
**         the selected edge of the input signal (on the input pin), the current
**         content of the counter register is written into the capture
**         register and the OnCapture event is called.
*/         
/*!
**  @addtogroup PWM_In_module PWM_In module documentation
**  @{
*/         

/* MODULE PWM_In. */

#include "Events.h"
#include "PWM_In.h"




/*
** ===================================================================
**     Method      :  PWM_In_Reset (component Capture)
**     Description :
**         This method resets the counter register (see implementation
**         note in <General Info>).
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte PWM_In_Reset(word *Value)

**  This method is implemented as a macro. See PWM_In.h file.  **
*/

/*
** ===================================================================
**     Method      :  PWM_In_GetCaptureValue (component Capture)
**     Description :
**         This method gets the last value captured by enabled timer.
**         Note: one tick of timer is
**               250 ns in high speed mode
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Value           - A pointer to the content of the
**                           capture register
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
/*
byte PWM_In_GetCaptureValue(PWM_In_TCapturedValue *Value)

**  This method is implemented as a macro. See PWM_In.h file.  **
*/

/*
** ===================================================================
**     Method      :  PWM_In_GetPinValue (component Capture)
**     Description :
**         The method reads the Capture pin value. The method is
**         available only if it is possible to read the pin value
**         (usually not available for internal signals).
**     Parameters  : None
**     Returns     :
**         ---             - Capture pin value.
**                           <true> - high level
**                           <false> - low level.
** ===================================================================
*/
/*
bool PWM_In_GetPinValue(void)

**  This method is implemented as a macro. See PWM_In.h file.  **
*/

/*
** ===================================================================
**     Method      :  PWM_In_Init (component Capture)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void PWM_In_Init(void)
{
  /* FTM0_SC: TOF=0,TOIE=0,CPWMS=0,CLKS=0,PS=0 */
  setReg8(FTM0_SC, 0x00U);             /* Stop HW */ 
  /* FTM0_MOD: MOD=0 */
  setReg16(FTM0_MOD, 0x00U);           /* Disable modulo register */ 
  /* FTM0_CNTH: COUNT_H=0 */
  setReg8(FTM0_CNTH, 0x00U);           /* Reset counter */ 
  /* FTM0_C1V: VAL=0 */
  setReg16(FTM0_C1V, 0x00U);           /* Clear capture register */ 
  /* FTM0_SC: PS=2 */
  clrSetReg8Bits(FTM0_SC, 0x05U, 0x02U); /* Set prescaler register */ 
  /* FTM0_C1SC: CHF=0,CHIE=1,MSB=0,MSA=0,ELSB=1,ELSA=1,??=0,??=0 */
  setReg8(FTM0_C1SC, 0x4CU);           /* Enable both interrupt and capture function */ 
  /* FTM0_SC: TOF=0 */
  clrReg8Bits(FTM0_SC, 0x80U);         /* Reset overflow interrupt request flag */ 
  /* FTM0_SC: TOIE=1 */
  setReg8Bits(FTM0_SC, 0x40U);         /* Enable overflow interrupt */ 
  /* FTM0_SC: CLKS=1 */
  clrSetReg8Bits(FTM0_SC, 0x10U, 0x08U); /* Run counter */ 
}


/*
** ===================================================================
**     Method      :  Interrupt (component Capture)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes event(s) of the component.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(PWM_In_Interrupt)
{
  (void)FTM0_C1SC;                     /* Dummy read to reset interrupt request flag */
  /* FTM0_C1SC: CHF=0 */
  clrReg8Bits(FTM0_C1SC, 0x80U);       /* Reset interrupt request flag */ 
  PWM_In_OnCapture();                  /* Invoke user event */
  IPC_SC_PULIPM = 1U;                  /* Restore Interrupt Priority Mask */
}



/*
** ===================================================================
**     Method      :  IntOverflow (component Capture)
**
**     Description :
**         The method services the interrupt of the selected peripheral(s)
**         and eventually invokes of the component event(s).
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
ISR(PWM_In_IntOverflow)
{
  (void)FTM0_SC;
  /* FTM0_SC: TOF=0 */
  clrReg8Bits(FTM0_SC, 0x80U);         /* Reset overflow interrupt request flag */ 
  PWM_In_OnOverflow();                 /* Invoke user event */
  IPC_SC_PULIPM = 1U;                  /* Restore Interrupt Priority Mask */
}



/* END PWM_In. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
