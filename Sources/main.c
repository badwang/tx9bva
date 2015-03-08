/* ###################################################################
**     Filename    : main.c
**     Project     : Test1
**     Processor   : MC9S08PA4VTG
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2014-12-20, 21:10, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "AD1.h"
#include "PWM_In.h"
#include "PWM_Timer.h"
#include "WDT.h"
#include "Backlight.h"
#include "Signal_In.h"
#include "Lever_Pos.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "main.h"

byte PWM_Out_Duty_Backlight=2, PWM_Out_Duty_Lever=2, PWM_Out_Lever=0;								//Set the output PWM duty. Max 255, Min 0.
bool PWM_In_Overflow=0, PWM_In_Ready=0, PWM_In_Always_On=0, PWM_In_Always_Off=0;					//Flags for PWM measurement.
word PWM_In_On=0, PWM_In_Off=24000;

byte Day_Mode=1;
byte Compensation_Factor=255;
byte Current_Duty_Cycle, Current_Levers;
byte Levers_Tmp[10]={0};
byte Levers_Tmp_Ptr=0;

#define DAY_MODE_DUTY_CYCLE_LEVER 150					//Output duty cycle for levers day mode
#define NIGHT_MODE_DUTY_CYCLE_BACKLIGHT 100				//Output duty cycle for backlight night mode
#define NIGHT_MODE_DUTY_CYCLE_LEVER 30					//Output duty cycle for levers night mode
#define DARK_MODE_DUTY_CYCLE 1							//Output duty cycle for dark mode

#define INPUT_V_MIN 95									//Pre-define the A/D result for when input voltage = 8V 

//Defines input & output bits pattern for levers.
#define IN_P 0b00000100
#define IN_R 0b00000110
#define IN_N 0b00000111
#define IN_D 0b00000011
#define IN_M 0b00000001

#define LeverP 0b10000
#define LeverR 0b01000
#define LeverN 0b00100
#define LeverD 0b00010
#define LeverM 0b00001

void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */

  
  //Main loop starts here.
  for(;;){
	  
	  Power_Calibration();								//Check the power supply voltage and calculate compensation factor.
	  
	  Current_Levers = Get_Levers();
	  
	  Current_Levers=LeverD;							//For debugging.
	  
	  if(Current_Levers == 0xFF){																//Wrong input signal combination detected.
		  //Turn off all lever and backlight LEDs.
		  Set_Output(0, 0, 0x0);
	  }else if(PWM_In_Always_On){																
		  //PWM_In always 1
		  Set_Output(NIGHT_MODE_DUTY_CYCLE_BACKLIGHT, NIGHT_MODE_DUTY_CYCLE_LEVER, Current_Levers); //Night mode. PWM in MAX.
	  }else if(PWM_In_Always_Off){															
		  //PWM_In always 0
		  Set_Output(50, DAY_MODE_DUTY_CYCLE_LEVER, Current_Levers); 										//Day mode.
	  }else if(PWM_In_Ready){																
		  //PWM_In duty measured OK w/o timer overflow
		  Current_Duty_Cycle = Get_Duty_Cycle();
		  Set_Output(NIGHT_MODE_DUTY_CYCLE_BACKLIGHT, NIGHT_MODE_DUTY_CYCLE_LEVER, Current_Levers);		//Night mode
	  }else{																				
		  //PWM_In frequency/duty is out of range.
		  Set_Output(NIGHT_MODE_DUTY_CYCLE_BACKLIGHT, NIGHT_MODE_DUTY_CYCLE_LEVER, Current_Levers);		//Set to night mode.
	  }
	  
	  (void) WDT_Clear();																		//Kick watch dog
  }
  
  
  
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */

void Set_Output(byte Backlight_Duty_Cycle, byte Lever_Duty_Cycle, byte Levers)
{
	word temp=0;
	
	temp = (word)Backlight_Duty_Cycle;
	temp = temp * Compensation_Factor / 256;
	PWM_Out_Duty_Backlight = (byte) temp;
	
	temp = (word)Lever_Duty_Cycle;
	temp = temp * Compensation_Factor / 256;
	PWM_Out_Duty_Lever = (byte) temp;
	
	PWM_Out_Lever=Levers;
}


void Power_Calibration(void)
{
	byte Result, Input_Voltage, i, dummy=0;
	word PC_Temp=0;
	
	//Do 4 cycles of A/D convert on input power supply and use the average value.
	for(i=0;i<4;i++){
		dummy = AD1_MeasureChan(1, 0);								//Do A/D convert on input power supply. Wait until A/D completed.
		dummy = AD1_GetChanValue8(0, &Result);						//Get the result byte.
		PC_Temp += (word)Result;
	}
	Input_Voltage = (byte) (PC_Temp/4);					//Get average of 4 results.

	dummy = AD1_MeasureChan(1, 1);								//Do A/D convert on internal reference. Wait until A/D completed.
	dummy = AD1_GetChanValue8(1, &Result);						//Get the result byte.
	
	if(Input_Voltage < INPUT_V_MIN){					//Check if Input Voltage < 8V
		Input_Voltage = INPUT_V_MIN;
	}
	
	PC_Temp = INPUT_V_MIN;
	PC_Temp = PC_Temp << 8;
	PC_Temp = PC_Temp / Input_Voltage;
	if(PC_Temp > 255){
		PC_Temp = 255;
	}
	
	PC_Temp = PC_Temp * PC_Temp;
	
	Compensation_Factor = (byte)(PC_Temp>>8);
	
	return;
}


byte Get_Duty_Cycle(void)
{
	word PWM_On, PWM_Period;
	word temp, result;
	
	PWM_On = PWM_In_On;
	PWM_Period = PWM_On + PWM_In_Off;
	
	//Invalid value check
	if(PWM_On == 0 || PWM_Period == 0){
		return DARK_MODE_DUTY_CYCLE;
	}
	
	//Invalid PWM period exceeds 65536*0.25us=16.384ms, e.g. < 61Hz 
	if(PWM_On >= PWM_Period){
		return DARK_MODE_DUTY_CYCLE;
	}
	
	//Invalid PWM period less than 10000*0.25us=2.5ms, e.g. > 400Hz 
	if(PWM_Period < 10000){
		return DARK_MODE_DUTY_CYCLE;
	}

	//Calculate native input duty cycle
	temp = PWM_Period & 0x00FF;
	if(temp > 127){
		temp = (PWM_Period>>8)+1;
	}else{
		temp = (PWM_Period>>8);
	}

	result = PWM_On / temp;								//Measured duty cycle = result / 256.
	
	return (byte)result;
}

byte Get_Levers(void)
{
	byte temp;
	byte i;
	
	temp = Signal_In_GetVal();
	
	Levers_Tmp[Levers_Tmp_Ptr]=temp;
	Levers_Tmp_Ptr++;
	if(Levers_Tmp_Ptr>9)Levers_Tmp_Ptr=0;
	
	for(i=0;i<10;i++){
		if(Levers_Tmp[i]!=Levers_Tmp[0]){
			temp=Current_Levers;
			break;
		}
		temp=Levers_Tmp[0];
	}
	
//	if(temp==Current_Levers)return temp;
	
	switch(temp){
	case IN_P:
		return LeverP;
		break;
	case IN_R:
		return LeverR;
		break;
	case IN_N:
		return LeverN;
		break;
	case IN_D:
		return LeverD;
		break;
	case IN_M:
		return LeverM;
		break;
	default:
		return 0xFF;
	}
	
	
}

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
