# Local Bus Network Protocol Implementation

This project contains an implementation of the local bus network protocol which can send different message types for configuring and facilitating communication between Sensors, PID controllers and Actuators.

The protocol is intended for use on Controller Area Networks (CAN) to communicate between different devices.

## The module

The module is comprised of four different parts

### canCom

canCom is the driver responsible for communicating with the MCP2515 hardware to be able to send and receive CAN messages

### circularbuffer

The MCP2515 can only store two received messages before it will start overwriting those when receiving new messages. Because of the possible high message frequency on the network and the duration between message handling by devices. When received an interrupt is used to quickly retrieve the message from the MCP2515 hardware and store it locally inside of a circular buffer.

The default circularbuffer size is 10 but can increased by defining MSG_BUFF_SIZE if memory limitations allow it. (Note: at some point a larger buffer wont help. If more and more messages pile up inside the buffer you need to give the messagehandler more cycles to be able to clear the backlog.)

### TEA

TEA or Tiny Encryption Algorithm is used to encrypt the messages that are send over the network. This mostly used to obfuscate the protocol and make it harder to reverse engineer.

When used all devices on the network must contain the same hardcoded key.

### LBNProtocol

Handles all of the initializing and interfacing necessary to be able to use the module and implements the protocol itself.

## Build system

This project uses the [platformio](https://platformio.org/) build system to compile and flash the firmware

## Usage 

To be able to use the LBNModule you need to do the following:

### Create a messagehandler

```
void msgHandler(canComMsg* msg){
    /* do stuff with the msg*/
}
```

### Initialize the LBNModule

```
lbnInit(<Node_ID>,&msgHandler);
```

### Send a message

```
LBNData data;
data.<data_type> = <value>;
sendMessage(<message_type>,&data);
```

See [example.cpp](src/example.cpp) for a practical example.