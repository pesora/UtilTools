//==================================================================
//        Sensor Tech Inc
// -----------------------------------------------------------------
//  PROJECT			: Wearable Sensor
//	PRODUCT NAME	: WeSen(Werable Sensor)
//	FILE NAME		: MovingAverage.h
//  PROCESSOR		: efm32GG990F1024
//  COMPILER		: IAR 6.3
//  AUTHOR			: Hwang Seongjin
//  DATE			  : 2016.11.17
//
//  DESCRIPTION		:
//  NOTES			:
//===================================================================
#ifndef MOVING_AVERATGE_H
#define MOVING_AVERATGE_H


#include <stdint.h>

typedef union tagWORD{
  uint16_t word;
  struct {
    uint8_t l;
    uint8_t h;
  };
}WORDDATA;

////////////////////////////////////////////////
// double CDCDATA
typedef struct tag_ENVDATA {
	float    fan_rpm;	   	//fan rpm
} ENVDATA_t;


#define MAX_FAN_CH        2
#define MAX_ENV_BUF_SIZE  10
////////////////////////////////////////////////
extern ENVDATA_t currEmaData[MAX_ENV_BUF_SIZE];  //current ema data
extern ENVDATA_t prevEmaData[MAX_ENV_BUF_SIZE];  //previous ema data

////////////////////////////////////////////////
extern void EMA_Init(uint8_t n);
ENVDATA_t GetEma(uint8_t n, uint8_t ch, ENVDATA_t data);

#endif
