#ifndef LBNPROTOCOL_H_
#define LBNPROTOCOL_H_

#include "canCom.h"
#ifdef LBN_UNITTEST
#include "../ArduinoMock.h"
#else
#include <Arduino.h>
#endif
#include "circularbuffer.h"
#define CIRCULAR_BUFFER_INT_SAFE

/**
 * Defines the options of a message type
 */
enum LBNProtocolMessageType
{
    NULL_TYPE,
    CONFIG_SET_RECIPIENT,
    CONFIG_SET_SENSOR_MEASURE_FREQUENCY,
    RAW_SENSOR_DATA,
    PROCESSED_SENSOR_DATA,
    SET_ACTUATOR_VALUE,
    CONFIG_SET_MAX_TURNS,
    CONFIG_SET_TOF_MARGIN,
    CONFIG_SET_TOF_MAX_DISTANCE,
    CONFIG_SET_PID_P,
    CONFIG_SET_PID_I,
    CONFIG_SET_PID_D,
    CONFIG_SET_PID_MIDDLE_POINT,
    CONFIG_TAKE_OVER,
    CONFIG_CHANGE_LISTENING,
    ECG_SENSOR_DATA
};


#ifndef MSG_BUFF_SIZE
#define MSG_BUFF_SIZE 10
#endif

/**
 * Inialiser that sets up the lbn protocol
 * 
 * Parameter:
 *  ID from this CAN module (unsigned short)
 *  Pointer of the msgHandler function
 */
void lbnInit(unsigned short id, void (*msgHandlerFunction)(canComMsg *));

/**
 * sets up protocolMessageTypeLengths 
 */
void setupTypeLengths();

/**
 * sends message of type with the required data for that message type
 * 
 * Parameter:
 *  the kind of message that you want to send (enum LBNProtocolMessageType)
 *  the content (union data)
 */
void sendMessage(LBNProtocolMessageType messageType, LBNData *content);

/**
 * receives CAN message and adds it to the circular buffer
 */
void receiveMessage();

/**
 * processes messages according to its message type
 */
void handleMessage();

/**
 * adds remote address to filters from which it can receive messages
 * 
 * Parameter:
 *   ID from the CAN module you want to receive messages from(unsigned short)
 */
void addRemoteId(unsigned short id);

#endif
