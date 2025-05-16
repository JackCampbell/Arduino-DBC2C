// Code Generator

#include <stdint.h>
#include <stdbool.h>
#include "VMS_CANGW_Functions_M4.h"
#include "VMS_CANGW_Protocol.h"

// CAN bus configuration type
CFG_CANCH_Type CFG_CANCH0;
CFG_CANCH_Type CFG_CANCH1;

GWLAN_DATA prog_data = { 0 };





uint32_t MakeN2K_ID( uint32_t PGN, uint8_t prio, uint8_t SA, uint8_t DA ) {
	uint32_t ID = ( prio & 0x07 ) << 26;  // priority
	if( ( ( PGN >> 8 ) & 0xFF ) < 240 ) { // dst address
		ID |= ( PGN & 0x3FF00 ) << 8;
		ID |= ( DA << 8 );
	} else {
		ID |= ( PGN & 0x3FFFF ) << 8; // DA => FF
	}
	ID |= SA | ( 1UL << 30 ); // src address and extended bits
	return ID;
}

uint32_t MakeID_N2K( uint32_t ID ) {
	uint8_t prio = (ID >> 26) & 0x07;
	uint8_t SA = ID & 0xFF;
	uint8_t DA = 0;
	if( ( ( ID >> 16 ) & 0xFF ) < 240 ) {
		DA = ( ID >> 8 ) & 0xFF;
		return ( ID >> 8 ) & 0x3FF00;
	} else {
		return ( ID >> 8 ) & 0x3FFFF;
	}
}

uint32_t MakeHolley_ID( uint32_t target, uint32_t serial, bool cmd ) {
	// Set extended ID bit (used in Chip_CCAN_Send to identify extended ID)
	uint32_t ID = ( 1UL << 30 );
	if( cmd ) {
		ID |= ( 1UL << 28 ); // command
	}
	ID |= ( 0x7 << 25 ); // target ID
	ID |= ( target & 0x7ff ) << 14;
	ID |= ( 0x4 << 11 );
	ID |= ( serial & 0x7ff );
	return ID;
}

uint32_t MakeID_Holley( uint32_t ID ) {
	uint8_t target = ( ID >> 14 ) & 0x7ff;
	uint8_t serial = ID & 0x7ff;
	return target;
}


void ConfigCH0(void) {
    // 0x9e005000 RPM : 
    CFG_CANCH0.CANMSG[0].ID = 0x9E005000;
    CFG_CANCH0.CANMSG[0].TYPE = CAN_EXT_ID;
    CFG_CANCH0.CANMSG[0].MASK = (0x3FFF << 11);
    CFG_CANCH0.CANMSG[0].DLC = 8;
    CFG_CANCH0.CANMSG[0].USE = true;
    CFG_CANCH0.CANMSG[0].TX = false;

}

void ReceiveFromCH0(CCAN_MSG_OBJ_T *msg_buf) {
    if( MASK_29BIT(msg_buf->id) == 0x9E005000 ) {
        // speed_value -> speed <- CH0:Complete Holley EFI export:9E005000:RPM
        uint32_t speed_raw = ;
        prog_data.speed_value = (*(float *)&speed_raw);
        prog_data.speed_timeout = 0;
        return;
    }
}

void ConfigCH1(void) {
}

void ReceiveFromCH1(CCAN_MSG_OBJ_T *msg_buf) {
}

// note: N2K 00 -> FF
void Send_AEMNetNEMA01_To_CH1() {
    uint32_t speed_raw = (uint32_t)(prog_data.speed_value);
    uint32_t speed_raw = (uint32_t)(prog_data.speed_value);
    uint32_t speed_raw = (uint32_t)(prog_data.speed_value);
    uint32_t speed_raw = (uint32_t)(prog_data.speed_value);

    CCAN_MSG_OBJ_T msg;
    msg.id = ID_29BIT(0x7c2, 0x100);
    msg.dlc = 8;
    msg.data[0] = 0xff;
    msg.data[1] = 0xff;
    msg.data[2] = 0xff;
    msg.data[3] = 0xff;
    msg.data[4] = 0xff;
    msg.data[5] = 0xff;
    msg.data[6] = 0xff;
    msg.data[7] = 0xff;
    Chip_CCAN_Send(LPC_C_CAN1, CCAN_MSG_IF2, false, &msg);
}

// Attr: GenMsgCyceTime
void SendMessageToChannels(uint32_t tick) {
    CHECK_TIMEOUT(speed_timeout, 1000);
    if( CYCLE_CHECK(25) ) {
        Send_AEMNetNEMA01_To_CH1();
    }
}

void DataSimulator(void) {
    if (!(LPC_GPIO_PORT->PIN[SIM_DIPSWITCH_GPIO_PORT] & (1UL << SIM_DIPSWITCH_GPIO_BIT))) { // check if  ECU simulator DIP switch is on
        // TODO
    }
}
    
