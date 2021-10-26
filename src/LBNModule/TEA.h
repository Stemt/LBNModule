#ifdef LBN_UNITTEST
#include "../ArduinoMock.h"
#else
#include <Arduino.h>
#endif
#include "canCom.h"

/**
 * This module implements the Tiny Encryption Algorithm (https://en.wikipedia.org/wiki/Tiny_Encryption_Algorithm)
 */

/**
 * The inialiser of the encryption
 * 
 * Parameter:
 *  The key of the encryption in an array of 5 keys, this can be random number with the max worth of 4294967295 (example = {2405922089, 2063307827, 1689751752, 680931428, 1162688827})
 */
void encryptInit(uint32_t pKey[5]);

/**
 * The encryption function
 * 
 * Parameter:
 *  The message you want to have encrypted in our canComMsg structure
 *  The returning encrypted message in a 64 bit format
 */
void encryptMsg(canComMsg* canComMsgP, uint32_t* encryptedMsg);

/**
 * The encryption function
 * 
 * Parameter:
 *  The message you want to have decrypted in our canComMsg structure
 *  The input encrypted message in a 64 bit format
 */
void decryptMsg(canComMsg* canComMsgP, uint32_t* encryptedMsg);