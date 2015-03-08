/*
 * main.h
 *
 *  Created on: Dec 30, 2014
 *      Author: zhengdw
 */

#ifndef MAIN_H_
#define MAIN_H_

void Set_Output(byte Backlight_Duty_Cycle, byte Lever_Duty_Cycle, byte Levers);
void Power_Calibration(void);
byte Get_Duty_Cycle(void);
byte Get_Levers(void);

#endif /* MAIN_H_ */
