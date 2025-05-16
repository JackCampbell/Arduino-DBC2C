// Code Generator

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#pragma once
#include <stdint.h>
#include <stdbool.h>

// (year-GW-devicenr) coded without dashes and using summation for the device et: nr -> 177188
#define SerialNrProg 217390

// File: Complete Holley EFI export Target:100
#define CAN_CH0_BAUDRATE  500000
#define CAN_CH0_SPT       87

// File: AEM-N2k Target:100
#define CAN_CH1_BAUDRATE  500000
#define CAN_CH1_SPT       80

typedef struct {
    // speed_value = speed <- CH0:Complete Holley EFI export:9E005000:RPM
    float                          speed_value;
    int                            speed_timeout;
} GWLAN_DATA;

GWLAN_DATA prog_data;

void ConfigCH0(void);
void ReceiveFromCH0(CCAN_MSG_OBJ_T *msg_buf);
void ConfigCH1(void);
void ReceiveFromCH1(CCAN_MSG_OBJ_T *msg_buf);
void SendMessageToChannels(uint32_t tick);
void DataSimulator(void);


#define MASK_29BIT(id) (id & (0x7FF << 14))
#define CLAMP_VALUE(value, min, max) (value < min ? min : (value > max ? max : value))
#define CYCLE_TIMEOUT(start, end) if(start < end) { start++; }
#define ID_29BIT(index, device) (0x4e002000 | ((index & 0x7ff) << 14) | (device & 0x7ff)) // (1UL << 28) -> command
// #define SIGNAL_VALID_TIMEOUT	1000
#endif

