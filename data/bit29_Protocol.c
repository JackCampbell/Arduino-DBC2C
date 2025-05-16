// Code Generator

#include <stdint.h>
#include <stdbool.h>
#include "VMS_CANGW_Functions_M4.h"
#include "VMS_CANGW_Protocol.h"

// CAN bus configuration type
CFG_CANCH_Type CFG_CANCH0;
CFG_CANCH_Type CFG_CANCH1;

GWLAN_DATA prog_data = { 0 };


void ConfigCH0(void) {
    // 0x96 PTEI_Trans_Clutch_Status_1 : 
    CFG_CANCH0.CANMSG[0].ID = 0x96;
    CFG_CANCH0.CANMSG[0].TYPE = CAN_STD_ID;
    CFG_CANCH0.CANMSG[0].MASK = 0;
    CFG_CANCH0.CANMSG[0].DLC = 6;
    CFG_CANCH0.CANMSG[0].USE = true;
    CFG_CANCH0.CANMSG[0].TX = false;
    // fuelcntr_cntr -> fuelcntr = 0 (Cntr)
    prog_data.fuelcntr_cntr = 0x0;
    // 0xc9 PPEI_Engine_General_Status_1 : Changed for PPEI 3.6
    CFG_CANCH0.CANMSG[1].ID = 0xC9;
    CFG_CANCH0.CANMSG[1].TYPE = CAN_STD_ID;
    CFG_CANCH0.CANMSG[1].MASK = 0;
    CFG_CANCH0.CANMSG[1].DLC = 8;
    CFG_CANCH0.CANMSG[1].USE = true;
    CFG_CANCH0.CANMSG[1].TX = false;
}

void ConfigCH1(void) {
    // 0x9e045000 FUEL_FLOW : 
    CFG_CANCH1.CANMSG[0].ID = 0x9E045000;
    CFG_CANCH1.CANMSG[0].TYPE = CAN_EXT_ID;
    CFG_CANCH1.CANMSG[0].MASK = (0x3FFF << 11);
    CFG_CANCH1.CANMSG[0].DLC = 8;
    CFG_CANCH1.CANMSG[0].USE = true;
    CFG_CANCH1.CANMSG[0].TX = false;
}

void ReceiveFromCH0(CCAN_MSG_OBJ_T *msg_buf) {
    if(msg_buf->id == 0x96) {
        // rpm_value -> rpm = [GlobalA_HS:96:TrnsInpDvcStat]
        int rpm_raw = (msg_buf->data[4] & 0x70);
        prog_data.rpm_value = CLAMP_VALUE(rpm_raw, 0, 7);
        prog_data.rpm_timeout = 0;
        return;
    }
    if(msg_buf->id == 0x0) {
        return;
    }
    if(msg_buf->id == 0xc9) {
        // AccAct_value -> AccAct = [GlobalA_HS:C9:AccActPos]
        int AccAct_raw = msg_buf->data[4];
        prog_data.AccAct_value = CLAMP_VALUE(0.392157 * AccAct_raw, 0, 100);
        prog_data.AccAct_timeout = 0;
        return;
    }
}


void DataSimulator(void) {
    if (!(LPC_GPIO_PORT->PIN[SIM_DIPSWITCH_GPIO_PORT] & (1UL << SIM_DIPSWITCH_GPIO_BIT))) { // check if  ECU simulator DIP switch is on
        // TODO
    }
}
	
