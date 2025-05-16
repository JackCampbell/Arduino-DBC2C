// Code Generator

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define SerialNrProg 217390 // (year-GW-devicenr) coded without dashes and using summation for the device nr -> 177188

#define CAN_CH0_BAUDRATE	500000
#define CAN_CH0_SPT	87

#define CAN_CH1_BAUDRATE	500000
#define CAN_CH1_SPT	80


typedef struct {
    // rpm_value -> demo:8CF004FF:EngineSpeed
    unsigned int                   rpm_value;
    int                            rpm_timeout;
} GWLAN_DATA;

GWLAN_DATA prog_data;

void ConfigCH0(void);
void ReceiveFromCH0(CCAN_MSG_OBJ_T *msg_buf);
void ConfigCH1(void);
void ReceiveFromCH1(CCAN_MSG_OBJ_T *msg_buf);
void SendMessageToChannels(int tick);
void DataSimulator(void);

#endif

