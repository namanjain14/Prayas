/***************************************************************************************/
/* Data Acquisition System  Project */
/* File Name: i2c_master.h */
/* Purpose: Declarations for variables required for i2c interface*/
/* Author:  Parijat Pathak(MAPYN)*/
/* Software Used: MPLAB 8.30, Mplab C32 compiler version:1.05 */
/***************************************************************************************/
#ifndef __I2C_master_H
#define __I2C_master_H
#include "TCPIP Stack/TCPIP.h"
//#include <pconfig.h>

void i2c_wait(unsigned int cnt);
void SetCurrentTime (void);
void GetCurrentTime (void);
#endif

