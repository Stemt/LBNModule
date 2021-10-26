#include <Arduino.h>
#include "CANModule/LBNProtocol.h"



void messageHandler(canComMsg* msg)
{
  Serial.print("message received: ");
        Serial.print("type=");
        Serial.print(msg->type);
        Serial.print(" data._byte=");
        Serial.println(msg->data._byte);
    switch (msg->type)
    {
    case SET_ACTUATOR_VALUE:
        Serial.print("type=RAW_SENSOR_DATA data._unsignedShort=");
        Serial.println(msg->data._unsignedShort);
        break;
    }
    delete msg;
}


/**
 * Node no. 1 sends messages to Node no. 2.
 */
#ifdef NODE_1
void setup(){
  Serial.begin(9600);
  lbnInit(0,messageHandler);
}

void loop() {
  handleMessage(); // the message handler should be called continiously or be triggered by the MCP2515 interrupt pin

  LBNData val; // to send data create the and LBNData variable
  val._unsignedShort = 10; // assign the variable with the correct corresponding data type
  sendMessage(SET_ACTUATOR_VALUE,&val);
  delay(500);
}
#endif

/**
 * Node no. 2 receives messages.
 */
#ifdef NODE_2
void setup(){
  Serial.begin(9600);
  Serial.println("beginning LBN");
  lbnInit(2,messageHandler);
  Serial.println("adding remote");
  addRemoteId(0);
}

void loop() {
  handleMessage();
}
#endif

