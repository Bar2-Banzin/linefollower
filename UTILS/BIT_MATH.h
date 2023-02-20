/***************************************************************************************/
/****************************  IMT School Training Center ******************************/
/***************************************************************************************/
/** This file is developed by IMT School training center, All copyrights are reserved **/
/***************************************************************************************/
#ifndef BIT_MATH_H
#define BIT_MATH_H

#define 	SET_BIT(REG_NAME,BIT_NUMBER) 		REG_NAME |= (1<<BIT_NUMBER)
#define 	CLEAR_BIT(REG_NAME,BIT_NUMBER) 		REG_NAME &= (~(1<<BIT_NUMBER))
#define 	TOGGLE_BIT(REG_NAME,BIT_NUMBER) 	REG_NAME ^= (1<<BIT_NUMBER)
#define     READ_BIT(REG_NAME,BIT_NUM)			(REG_NAME>>BIT_NUM)&1
#define     CLEAR_PORT(PORT_NAME)				PORT_NAME &=0x00
#define     SET_PORT(PORT_NAME)					PORT_NAME |=0xff
#define     READ_PORT(PORT_NAME)				PORT_NAME & 0xff

#endif
