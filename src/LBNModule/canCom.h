#ifndef CAN_COM_H_
#define CAN_COM_H_

#ifdef LBN_UNITTEST
#include "../ArduinoMock.h"
#else
#include <Arduino.h>
#endif

const unsigned char canMsgOverheadSize = 2;
const unsigned short maxAmountOfFilters = 6;

/**
 * Defines the options of the data in a can message
 */
typedef union
{
    byte _byte;
    byte _byteArray[7]; // a maximum of 7 bytes can be send at once
    unsigned short _unsignedShort;
    short _signedShort;
    unsigned long _unsignedLong;
    long _signedLong;
    float _float;
    struct
    {
        short data1;
        short data2;
        short data3;
    } ecgData;
    struct
    {
        unsigned short id;
        union
        {
            byte _byte;
            unsigned short _unsignedShort;
            short _signedShort;
            unsigned long _unsignedLong;
            long _signedLong;
            float _float;
        } data;
    } configMsg;
} LBNData;

/**
 * The structure of a can message
 */
typedef struct
{
    byte type;
    LBNData data;
} canComMsg;

enum whatMask
{
    bothMasks,
    mask0,
    mask1
};

/**
 * call this function to init without interrupt pin
 *
 * Parameter:
 *  The speed of the CAN (byte speed)
 */
void canComInit(byte speed);

/**
 * call this function to send a canComMsg
 * 
 * Parameter:
 *  The id of the sending CAN
 *  The message you want to send
 *  The length of the message
 * 
 * Return:
 *  An error message
 */
byte canComSend(unsigned short id, canComMsg *msg, byte length);

/**
 * call this function to receive a canComMsg
 * returns 0 if succesful other numbers correspond to different error of the CAN bus shield library
 * 
 * Parameter:
 *  The received message (this will be set to the pointer)
 *  The length of the received message (this will be set tot the pointer)
 * 
 * Return:
 *  An error message
 */
byte canComReceive(canComMsg *msg, byte *length);

/**
 * call this function to check if a message can be received
 */
bool canComCheckReceive();

/**
 * call this function to change the mask 
 * 
 * Parameter:
 *  this will define what bits will looked at (unsigned short maskId)
 *  which of the available masks u want to set (unsigned short whichMask)
 *  if it is the extended format, 0 = not extended(unsigned short idType)
 */
void changeMask(unsigned short maskId, unsigned short whichMask, unsigned short idType);

/**
 * call this function to add a filter
 * 
 * Parameter:
 *  what CAN id you want to filter out (unsigned short filterId)
 *  if it is the extended format, 0 = not extended(unsigned short idType)
 * 
 * Return:
 *  If the filter has been succesfully added
 */
bool addFilter(unsigned short filterId, unsigned short idType);

/**
 * call this function to change a certain filter
 * 
 * Parameter:
 *  what CAN id you want to filter out (unsigned short filterId)
 *  What filter you want to change
 *  if it is the extended format, 0 = not extended(unsigned short idType)
 */
void changeFilter(unsigned short filterId, unsigned short whatFilter, unsigned short idType);

/**
 * call this function to enable can message filtering
 */
void enableFiltering();

/**
 * call this function to disable can message filtering
 */
void disableFiltering();

#endif