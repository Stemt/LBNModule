#include <Arduino.h>
#include <SPI.h>
#include "canCom.h"
#include <mcp2515_can.h>

mcp2515_can can(10);
bool canInterruptEnabled = false;
bool canInterruptTriggered = false;

unsigned short amountOffilters = 0; 

void CAN_ISR(){
    canInterruptTriggered = true;
    Serial.println("interrupt");
}


void canComInit(byte speed){
    Serial.println("init CAN");
    if(!can.begin(speed)){
        Serial.println("succesful");
    }else{
        Serial.println("failed retrying");
        canComInit(speed);
    }
}

byte canComSend(unsigned short id,canComMsg *msg, byte length){
    return can.sendMsgBuf(id,0,length,(byte*) msg);
}


byte canComReceive(canComMsg *msg, byte* length){
    canInterruptTriggered = false;
    return can.readMsgBuf(length,(byte*) msg);
}


bool canComCheckReceive(){
    return can.checkReceive() == CAN_MSGAVAIL;
}

void changeMask(unsigned short maskId, unsigned short whichMask, unsigned short idType){  
    if(whichMask == bothMasks ){
        can.init_Mask(0, idType, maskId);
        can.init_Mask(1, idType, maskId);
    }
    else if (whichMask == mask0 ){
        can.init_Mask(0, idType, maskId);
    }
    else if (whichMask == mask1 ){
        can.init_Mask(1, idType, maskId);
    }    
}

bool addFilter(unsigned short filterId, unsigned short idType){
    
    if(amountOffilters < maxAmountOfFilters){
        can.init_Filt(amountOffilters, idType, filterId);
        ++amountOffilters;
        return true;
    }
    return false;
}

void changeFilter(unsigned short filterId,unsigned short whatFilter ,unsigned short idType){
    can.init_Filt(whatFilter, idType, filterId);
}

void enableFiltering(){
    changeMask(0x3ff, bothMasks, 0);
}

void disableFiltering(){
    changeMask(0, bothMasks, 0);
}