/* ###################################################################
**     Filename    : Events.c
**     Project     : Test1
**     Processor   : MC9S08PA4VTG
**     Component   : Events
**     Version     : Driver 01.02
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2014-12-20, 21:10, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.02
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

static byte PWM_Int_Counter=0, PWM_In_Overflow_Time=0;
extern byte PWM_Out_Duty_Backlight, PWM_Out_Duty_Lever, PWM_Out_Lever;
extern bool PWM_In_Overflow, PWM_In_Ready, PWM_In_Always_On, PWM_In_Always_Off;
extern word PWM_In_On, PWM_In_Off;

#define PWM_IN_OVERFLOW_MAX 3							//3x16=48mS. Time to detect constant PWM_Input 1 or 0.


/*
** ===================================================================
**     Event       :  PWM_In_OnCapture (module Events)
**
**     Component   :  PWM_In [Capture]
**     Description :
**         This event is called on capturing of Timer/Counter actual
**         value (only when the component is enabled - <Enable> and the
**         events are enabled - <EnableEvent>.This event is available
**         only if a <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PWM_In_OnCapture(void)
{
  /* Write your code here ... */
	PWM_In_Reset();
	
	if(PWM_In_GetPinValue()){								//Check the event happened on rising r falling edge.
		PWM_In_GetCaptureValue(&PWM_In_Off);				//Get the PWM input low time.
	}else{
		PWM_In_GetCaptureValue(&PWM_In_On);					//This is falling edge. Get the PWM input high time.
	}
	
	if(PWM_In_Overflow){								//Check if timer overflow already happened.
		PWM_In_Ready=0;									//Overflow happened, non valid measurement.
		PWM_In_Overflow=0;
	}else{
		PWM_In_Ready=1;									//No timer overflow, valid measurement.
	}

	PWM_In_Overflow_Time=0;
	PWM_In_Always_Off=0;
	PWM_In_Always_On=0;
}

/*
** ===================================================================
**     Event       :  PWM_In_OnOverflow (module Events)
**
**     Component   :  PWM_In [Capture]
**     Description :
**         This event is called if counter overflows (only when the
**         component is enabled - <Enable> and the events are enabled -
**         <EnableEvent>.This event is available only if a <interrupt
**         service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PWM_In_OnOverflow(void)
{
  /* Write your code here ... */
	PWM_In_Overflow=1;
	
	if(PWM_In_Overflow_Time < PWM_IN_OVERFLOW_MAX){
		PWM_In_Overflow_Time++;
	}else{
		if(PWM_In_GetPinValue()){
			PWM_In_Always_Off=0;
			PWM_In_Always_On=1;
		}else{
			PWM_In_Always_Off=1;
			PWM_In_Always_On=0;
		}
	}
}


/*
** ===================================================================
**     Event       :  PWM_Timer_OnInterrupt (module Events)
**
**     Component   :  PWM_Timer [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PWM_Timer_OnInterrupt(void)
{
  /* Write your code here ... */
	
	if(PWM_Int_Counter >= PWM_Out_Duty_Backlight){
//	if(PWM_Int_Counter >= 20){
		Backlight_PutVal(0);
	}else{
		Backlight_PutVal(1);
	}

	if(PWM_Int_Counter >= PWM_Out_Duty_Lever){
//	if(PWM_Int_Counter >= 30){
		Lever_Pos_PutVal(0);
	}else{
		Lever_Pos_PutVal(PWM_Out_Lever);
	}

	PWM_Int_Counter++;
	
}

/*
** ===================================================================
**     Event       :  Cpu_OnSwINT (module Events)
**
**     Component   :  Cpu [MC9S08PA4_16]
**     Description :
**         This event is called when the SWI interrupt had occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void Cpu_OnSwINT(void)
{
  /* Write your code here ... */
	//Dead loop here to trigger watchdog.
	for(;;);
}

/* END Events */

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
