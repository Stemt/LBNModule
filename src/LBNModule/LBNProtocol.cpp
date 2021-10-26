#include "LBNProtocol.h"
#include "TEA.h"
#ifndef LBN_UNITTEST
#include <mcp2515_can.h>
#endif

unsigned short id;
unsigned short subscriptionList[7];
unsigned short subscriptionListIndex;
unsigned short protocolMessageTypeLengths[16];
void (*msgHandler)(canComMsg *);
cbuffer* msgBuff;

//hardcoded key
uint32_t encryptionKey[5] = {12,12,12,12,12};

void lbnInit(unsigned short idParameter, void (*msgHandlerFunction)(canComMsg *))
{
    Serial.println("Setting variables LBN");
    id = idParameter;
    msgHandler = msgHandlerFunction;
    encryptInit(encryptionKey);
    msgBuff = cbInit(MSG_BUFF_SIZE, OVERWRITE_IF_FULL);
    Serial.println("initting LBN");
    setupTypeLengths();
    Serial.println("type lengths setup");
    canComInit(CAN_1000KBPS);
    enableFiltering();
    Serial.println("filtering enabled");
    attachInterrupt(digitalPinToInterrupt(2), receiveMessage, FALLING);
    Serial.println("interrupt attached");
}

void setupTypeLengths()
{
    protocolMessageTypeLengths[NULL_TYPE] = sizeof(unsigned short);
    protocolMessageTypeLengths[CONFIG_SET_RECIPIENT] = sizeof(unsigned short) * 2;
    protocolMessageTypeLengths[CONFIG_SET_SENSOR_MEASURE_FREQUENCY] = sizeof(unsigned short) * 2;
    protocolMessageTypeLengths[RAW_SENSOR_DATA] = sizeof(unsigned short);
    protocolMessageTypeLengths[PROCESSED_SENSOR_DATA] = sizeof(unsigned short);
    protocolMessageTypeLengths[SET_ACTUATOR_VALUE] = sizeof(unsigned short);
    protocolMessageTypeLengths[CONFIG_SET_MAX_TURNS] = sizeof(unsigned long)  + sizeof(unsigned short);
    protocolMessageTypeLengths[CONFIG_SET_TOF_MARGIN] = sizeof(short) + sizeof(unsigned short);
    protocolMessageTypeLengths[CONFIG_SET_TOF_MAX_DISTANCE] = sizeof(unsigned short) * 2;
    protocolMessageTypeLengths[CONFIG_SET_PID_P] = sizeof(float) + sizeof(unsigned short);
    protocolMessageTypeLengths[CONFIG_SET_PID_I] = sizeof(float) + sizeof(unsigned short);
    protocolMessageTypeLengths[CONFIG_SET_PID_D] = sizeof(float) + sizeof(unsigned short);
    protocolMessageTypeLengths[CONFIG_SET_PID_MIDDLE_POINT] = sizeof(float) + sizeof(unsigned short);
    protocolMessageTypeLengths[CONFIG_TAKE_OVER] = sizeof(unsigned short) * 2;
    protocolMessageTypeLengths[CONFIG_CHANGE_LISTENING] = sizeof(unsigned short) * 2;
    protocolMessageTypeLengths[ECG_SENSOR_DATA] = sizeof(short) * 3;
}

void addRemoteId(unsigned short remoteId)
{
    // 7 remote ID's are the limit
    if (subscriptionListIndex < 7)
    {
        subscriptionList[subscriptionListIndex] = id;
        addFilter(remoteId, 0);
    }
}

void sendMessage(LBNProtocolMessageType type, LBNData *content)
{
    canComMsg msg;
    canComMsg encMsg;
    msg.type = type;
    msg.data = *content;
    //Serial.println("encrypting ");
    // byte arr[] = {10,0,0,0,0,0,0};
    // for(int i = 0; i < 7; i++){
    //     msg.data._byteArray[i] = arr[i];
    // }
    Serial.println(msg.data._unsignedShort);
    encryptMsg(&msg,(uint32_t*) &encMsg);
    // encMsg = msg;
    Serial.println(encMsg.data._unsignedShort);

    
    canComSend(id, &encMsg, 8 );//protocolMessageTypeLengths[type] + 1); //added 1 to type length to account for sending the type byte itself
}

void receiveMessage()
{
    canComMsg msg;
    canComMsg encMsg;
    byte length = 0;
    while (canComCheckReceive())
    {
        canComReceive(&encMsg, &length);
        Serial.println(encMsg.data._unsignedShort);

        decryptMsg(&msg, (uint32_t*) &encMsg);
        //msg = encMsg;
        Serial.println(msg.data._unsignedShort);

        cbAdd(msgBuff, msg);
        Serial.println("\ni");
    }
}

void handleMessage()
{
    if (cbAvailable(msgBuff))
    {
        canComMsg msg = cbRead(msgBuff);
        msgHandler(&msg);
    }
}