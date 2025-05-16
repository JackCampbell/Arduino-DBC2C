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
    // 0x0c9 PPEI_Engine_General_Status_1
    // Changed for PPEI 3.6
    CFG_CANCH0.CANMSG[0].ID = 0x0C9;
    CFG_CANCH0.CANMSG[0].TYPE = CAN_STD_ID;
    CFG_CANCH0.CANMSG[0].MASK = 0;
    CFG_CANCH0.CANMSG[0].DLC = 8;
    CFG_CANCH0.CANMSG[0].USE = true;
    CFG_CANCH0.CANMSG[0].TX = false;
}

void ConfigCH1(void) {
}

void ReceiveFromCH0(CCAN_MSG_OBJ_T *msg_buf) {
    if(msg_buf->id == 0x0c9) {
        // rpm_value -> demo:8CF004FF:EngineSpeed
        int rpm_raw = (msg_buf[1] & 0x80)|(msg_buf[2] << 8)|((msg_buf[3] & 0x7f) << 16);
        pr_data.rpm_value = clamp(prog_data.rpm_raw / 4, 0, 16383.8);
        pr_data.rpm_timeout = 0;
        // rpm_value -> demo:8CF004FF:EngineSpeed
        int rpm_raw = (msg_buf[1] & 0x80)|(msg_buf[2] << 8)|((msg_buf[3] & 0x7f) << 16);
        pr_data.rpm_value = clamp(prog_data.rpm_raw / 4, 0, 16383.8);
        pr_data.rpm_timeout = 0;
        return;
    }
}

void ReceiveFromCH1(CCAN_MSG_OBJ_T *msg_buf) {
}

void Send_EEC1() {
    int rpm_raw = (int)((prog_data.rpm_raw * 8));

    CCAN_MSG_OBJ_T msg;
    msg.id = 0x8CF004FF;
    msg.dlc = 8;
    msg.data[0] = 0x00;
    msg.data[1] = 0x00;
    msg.data[2] = 0x00;
    msg.data[3] = rpm_raw & 0xff;
    msg.data[4] = (rpm_raw >> 8) & 0xff;
    msg.data[5] = 0x00;
    msg.data[6] = 0x00;
    msg.data[7] = 0x00;
    Chip_CCAN_Send(LPC_C_CAN1, CCAN_MSG_IF2, false, &msg);
}

void Send_CCVS1() {
    int naum_raw = (int)(0x3F);

    CCAN_MSG_OBJ_T msg;
    msg.id = 0x98FEF1FE;
    msg.dlc = 8;
    msg.data[0] = 0x00;
    msg.data[1] = (naum_raw >> 16) & 0x01;
    msg.data[2] = (naum_raw >> 8) & 0xff;
    msg.data[3] = naum_raw & 0xfe;
    msg.data[4] = 0x00;
    msg.data[5] = 0x00;
    msg.data[6] = 0x00;
    msg.data[7] = 0x00;
    Chip_CCAN_Send(LPC_C_CAN1, CCAN_MSG_IF2, false, &msg);
}

void SendToCH0(int tick) {
    CYCLE_TIMEOUT( pr_data.rpm_timeout, 1000);
    if( CYCLE_CHECK(12) ) {
        Send_EEC1();
        Send_CCVS1();
    }
}

void DataSimulator(void) {
    if (!(LPC_GPIO_PORT->PIN[SIM_DIPSWITCH_GPIO_PORT] & (1UL << SIM_DIPSWITCH_GPIO_BIT))) { // check if  ECU simulator DIP switch is on
        // TODO
    }
}
	
