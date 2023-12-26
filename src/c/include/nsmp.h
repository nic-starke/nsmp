/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                  Copyright (c) (2023 - 2023) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#pragma once
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stdint.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define NSMP_PROTOCOL_VERSION 0x01

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Extern ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef enum {
	NSMP_NSG_TYPE_CTL_ACK,
	NSMP_MSG_TYPE_CTL_PLS_RETRY,

	NSMP_MSG_TYPE_CTL_DISCOVERY,
	NSMP_MSG_TYPE_CTL_REGISTER,
	NSMP_MSG_TYPE_CTL_UNREGISTER,

	NSMP_MSG_TYPE_USER_MESSAGE,
	NSMP_MSG_TYPE_NB,
} nsmp_message_type_e;

typedef enum {
	DEVICE_TYPE_DEVICE = 0x00,
	DEVICE_TYPE_NODE	 = 0x01,
} nsmp_device_type_e;

typedef struct {
	uint16_t protocol_version : 3;
	uint16_t message_type			: 4;
	uint16_t retransmission		: 1; // 0 = first transmission, 1 = retransmission
	uint16_t request_response : 1; // 0 = request, 1 = response
} nsmp_control_word_s;

typedef struct {
	uint8_t device_type : 1; // 0 = device, 1 = node
	uint8_t node_addr		: 3;
	uint8_t dev_addr		: 4;
} nsmp_routing_byte_s;

typedef struct __attribute__((packed)) {
	uint16_t control;
	uint8_t	 routing;
	uint8_t	 destination;
	uint8_t	 source;
	uint8_t	 crc8;
	uint8_t	 data_len;
} nsmp_header_s;

typedef struct {
	nsmp_header_s header;
	uint8_t*			data;
} nsmp_message_s;

enum {
	NSMP_ERR_BAD_ARG = -1,
	NSMP_ERR_BAD_CRC = -2,
	NSMP_ERR_BAD_LEN = -3,
	NSMP_ERR_BAD_VER = -4,

	NSMP_OK = 0,
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Variables ~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Functions ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Functions ~~~~~~~~~~~~~~~~~~~~~~~~~ */
