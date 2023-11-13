# Nic's Simple (Serial) Messaging Protocol

A simple extended star-topology serial messaging protocol designed for constrained systems (embedded).
In peer-to-peer connections no broker is used. When more than 2 peers are detected in the network then a broker is assigned.

## Features / Spec

- Little endian.
- Byte based with padding to align upto 32-bits.
- Includes source and destination ID (for broker-based networks)
- Control Id

## Why another protocol

- It's nice to work on designing systems from the ground up.
- No need to read 'many hundreds of pages' protocol spec to understand it.

## Frames


[Sync Byte] [Header] [Data (optional)] [Padding]


## Sync Byte

The sync byte used to separate messages in the stream.
If no escape sequence is used then the parser interprets it as a sync byte.
If there is an escape sequence (stuffing byte) then the parser interprets it as data in the message.


## Header

Byte Number | Description

[0] 	| Control Byte
[1-2] | Routing
[3] 	| 8-bit CRC
[4] 	| Data Len
[5] 	|	Data Block


### Byte 0 - Control Byte (bitfield)

Bit Number | Description

[0] | Request or Response (0 for request, 1 for response)
[1] | Includes Data
[x-7] | Message Type

---- Message Types
	0 = User Message
	1 = NSMP Discovery (ping)
	2 = NSMP Register
	3 = NSMP Bye!

### Byte <1,2> - Routing

[1] Destination
[2] Source

Both are the same:

<0> Broker bit (0 for peer, 1 for broker)
<1,2> Node Address (upto 4 nodes)
<3,7> Peer Address (upto 32 peers per node)

### CRC8

## NSMP Messages

### Discovery (PING)

**REQUEST**

If the local device wishes to connect to the network it must send a discovery ping.
This allows it to determine how many peers are in the network.

The local device sends the discovery request on the line:

- If a broker responds then the peer must register and obtain an address from the broker.
  - The peer must use this address for all future communications.
- If another peer responds then the local device can choose any address it likes as long as it does not match the responders' address.

**RESPONSE**
A receipt of a discovery request means that the remote peer/broker is connecting for the first time.
If this is the first of such handshakes then continue as normal.
If this is not the first and the local device already has an address then the remote device has either been reset, or was swapped/changed for a different device.

A broker can check incoming responses to determine if a peer is new, or was reset. If the message was received on an assigned serial port then the broker
knows that the remote peer has been reset, or was swapped out.

The broker responds as normal, ensuring the "broker bit" is set in the source byte of the "routing" part the message.
The peer can then send a register message (described later) if it wishes to join the network.

**WARNING**

- The local device MUST wait a random period between 10 and 50ms before sending the discovery request.
  - The wait reduces the chance of a race condition.
  - Correct configuration of a true RNG source is required by the system.
- It is always the responsibility of the local device to send the discovery and obtain/calculate an address if it wishes to connect to the network.
  - You can periodically transmit discovery requests, or wait for a detection of "link-status" changes on the serial device.

### Register

When the local device connects to a network for the first time it sends a discovery.

If it receives a response from peer then it knows it is not part of broker-based network. It should just modify its address to avoid collision with the remote device.
If it receives a response from a broker then it must register.

A registration occurs in the following manner:

1. Peer sends Discovery request
2. Broker sends Discovery response
3. Peer sends Register request
   1. The message includes data, the peer sets the data bit in the "Control" byte of the message header.
   2. The data is a 4 byte value of the devices UUID (serial number, part of its MAC address... etc)
4. Broker sends a Register response
   1. The responses' "Destination" address byte in the "routing" part of the message contains the address assigned to the peer.
5. The peer receives the brokers' Register response, and it extracts the destination address and assigns it to itself.


The broker will determine if the peer requires a new address, or a previously assigned address. This is based on the UUID sent by the .