/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                  Copyright (c) (2023 - 2023) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "nsmp.h"
#include "nsmp_parser.h"
#include "cobs.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define CONTROL_MASK_REQUEST_RESPONSE(x) ((x) & 0x01)
#define CONTROL_MASK_HAS_DATA(x)				 (((x) & 0x01) >> 1) // deprecated
#define CONTROL_MASK_MESSAGE_TYPE				 (((x) & 0xFC) >> 2)
#define ROUTING_MASK_DEVICE_TYPE(x)			 ((x) & 0x01)
#define ROUTING_MASK_NODE_ADDRESS(x)		 (((x) & 0x02) >> 1)
#define ROUTING_MASK_DEVICE_ADDRESS(x)	 (((x) & 0xFC) >> 2)

#define NSMP_HEADER_SIZE sizeof(nsmp_header_s)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Extern ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef struct __attribute__((packed)) {
	uint8_t control;
	uint8_t destination;
	uint8_t source;
	uint8_t crc8;
	uint8_t data_len;
} nsmp_header_s;

typedef struct {
	nsmp_header_s header;
	uint8_t*			data;
} nsmp_message_s;

typedef struct {
	uint8_t offset;
	uint8_t mask;
} nsmp_field_s;

typedef enum {
	MESSAGE_TYPE_USER_MESSAGE,
	MESSAGE_TYPE_DISCOVERY,
	MESSAGE_TYPE_REGISTER,
	MESSAGE_TYPE_BYE,

	MESSAGE_TYPE_NB,
} nsmp_message_type_e;

typedef enum {
	DEVICE_TYPE_DEVICE = 0x00,
	DEVICE_TYPE_NODE	 = 0x01,
} nsmp_device_type_e;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Variables ~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Functions ~~~~~~~~~~~~~~~~~~~~~~~~ */

int nsmp_parse(uint8_t* buffer, size_t buffer_len, nsmp_message_s* msg) {
	if (buffer == NULL || msg == NULL || buffer_len == 0 ||
			buffer_len < NSMP_HEADER_SIZE) {
		return -1;
	}

	msg->header.control			= buffer[offsetof(nsmp_header_s, control)];
	msg->header.destination = buffer[offsetof(nsmp_header_s, destination)];
	msg->header.source			= buffer[offsetof(nsmp_header_s, source)];
	msg->header.crc8				= buffer[offsetof(nsmp_header_s, crc8)];
	msg->header.data_len		= buffer[offsetof(nsmp_header_s, data_len)];
	msg->data = (msg->header.data_len > 0) ? &buffer[NSMP_HEADER_SIZE] : NULL;
	return 0;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Functions ~~~~~~~~~~~~~~~~~~~~~~~~~ */
