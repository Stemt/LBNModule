#ifdef LBN_UNITTEST
#include "../ArduinoMock.h"
#else
#include <Arduino.h>
#endif
#include "encrypt.h"


uint32_t key[5];

union EncryptMsg
{
    canComMsg msg;
    uint32_t data[2];
};

void encrypt(uint32_t v[2], uint32_t* r) {

    uint32_t v0=v[0], v1=v[1], sum=0, delta=key[4];
    v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    sum += delta;
    v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);

    r[0]=v0;
    r[1]=v1;
}

void decrypt(uint32_t* r, uint32_t v[2]) {

    uint32_t v0=v[0], v1=v[1], delta=key[4], sum=delta;
    v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    sum -= delta;
    v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);

    r[0]=v0;
    r[1]=v1;
}

void encryptMsg(canComMsg* canComMsgP, uint32_t* encryptedMsg) {

    EncryptMsg rawMsg;
    rawMsg.msg = *canComMsgP;

    encrypt(rawMsg.data, encryptedMsg);

    Serial.println("Encrypted succesfully");
}

void decryptMsg(canComMsg* canComMsgP, uint32_t* encryptedMsg) {

    EncryptMsg rawMsg;

    decrypt(rawMsg.data, encryptedMsg);

    *canComMsgP = rawMsg.msg;
    Serial.println("Decrypted succesfully");
}

void encryptInit(uint32_t pKey[5]){
    Serial.println("The encryption has been set up");
    memcpy(key, pKey, sizeof(uint32_t)*5);
}