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
    // rpm_value -> rpm = [GlobalA_HS:96:TrnsInpDvcStat]
    unsigned int                   rpm_value;
    int                            rpm_timeout;
    // fuel_value -> fuel = [Complete Holley EFI export:9E045000:FUEL_FLOW]
    float                          fuel_value;
    int                            fuel_timeout;
    // fuelcntr_cntr -> fuelcntr = 0 (Cntr)
    uint8_t                        fuelcntr_cntr;
    // AccAct_value -> AccAct = [GlobalA_HS:C9:AccActPos]
    unsigned int                   AccAct_value;
    int                            AccAct_timeout;
} GWLAN_DATA;

GWLAN_DATA prog_data;

void ConfigCH0(void);
void ReceiveFromCH0(CCAN_MSG_OBJ_T *msg_buf);
void ConfigCH1(void);
void ReceiveFromCH1(CCAN_MSG_OBJ_T *msg_buf);
void SendMessageToChannels(uint32_t tick);
void DataSimulator(void);

#define MASK_29BIT(x) (X & (0x7FF << 14))
#endif

