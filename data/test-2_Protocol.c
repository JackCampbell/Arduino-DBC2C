// Code Generator

#include <stdint.h>
#include <stdbool.h>
#include "VMS_CANGW_Functions_M4.h"
#include "VMS_CANGW_Protocol.h"

// CAN bus configuration type
CFG_CANCH_Type CFG_CANCH0;
CFG_CANCH_Type CFG_CANCH1;

GWLAN_DATA prog_data = { 0 };



void DataSimulator(void) {
    if (!(LPC_GPIO_PORT->PIN[SIM_DIPSWITCH_GPIO_PORT] & (1UL << SIM_DIPSWITCH_GPIO_BIT))) { // check if  ECU simulator DIP switch is on
        // TODO
    }
}
	
