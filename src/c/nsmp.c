/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                  Copyright (c) (2023 - 2023) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "nsmp.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#error "calculate masks"
#define CONTROL_MASK_REQUEST_RESPONSE(x) (x)
#define CONTROL_MASK_PROTOCOL_VERSION(x) (x)
#define CONTROL_MASK_MESSAGE_TYPE(x)		 (x)
#define ROUTING_MASK_DEVICE_TYPE(x)			 (x)
#define ROUTING_MASK_NODE_ADDRESS(x)		 (x)
#define ROUTING_MASK_DEVICE_ADDRESS(x)	 (x)

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Extern ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int parse_msg(uint8_t* buffer, size_t buffer_len, nsmp_message_s* msg,
										 bool write_data);
static int transmit(nsmp_message_s* msg);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Variables ~~~~~~~~~~~~~~~~~~~~~~~~~ */

static uint8_t local_address = 0;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Functions ~~~~~~~~~~~~~~~~~~~~~~~~ */

int nsmp_receive(uint8_t* rx, size_t rx_len) {
	if (rx == NULL || rx_len == 0) {
		return NSMP_ERR_BAD_ARG;
	}

	if (rx_len < sizeof(nsmp_header_s)) {
		return NSMP_ERR_BAD_ARG;
	}

	nsmp_message_s msg = {0};
	int						 err = parse_msg(rx, rx_len, &msg, false);

	switch (err != NSMP_OK)
		goto error;

	// Validate protocol version
	if (CONTROL_MASK_PROTOCOL_VERSION(msg.header.control) !=
			NSMP_PROTOCOL_VERSION) {
		err = NSMP_ERR_BAD_VER;
	}

error:

	switch (err) {
		case NSMP_ERR_BAD_CRC: {
			nsmp_message_s reply = {0};
			reply.header.control |=
					CONTROL_MASK_PROTOCOL_VERSION(NSMP_PROTOCOL_VERSION);
			reply.header.control |=
					CONTROL_MASK_MESSAGE_TYPE(NSMP_MSG_TYPE_CTL_PLS_RETRY);
			reply.header.destination = msg.header.source;
			reply.header.source			 = local_address;
			reply.header.data_len		 = 0;
			reply.header.crc8				 = crc_8((uint8_t*)&reply, sizeof(nsmp_header_s));
			err											 = transmit(&reply);
			goto done;
		}

		case NSMP_ERR_BAD_VER: goto done;
	}

done:

	return err;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Functions ~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int parse_msg(uint8_t* buffer, size_t buffer_len, nsmp_message_s* msg,
										 bool write_data) {
	if (buffer == NULL || msg == NULL || msg->data == NULL || buffer_len == 0 ||
			buffer_len < sizeof(nsmp_header_s)) {
		return NSMP_ERR_BAD_ARG;
	}

	// Copy header to message
	msg->header.control			= buffer[offsetof(nsmp_header_s, control)];
	msg->header.destination = buffer[offsetof(nsmp_header_s, destination)];
	msg->header.source			= buffer[offsetof(nsmp_header_s, source)];
	msg->header.crc8				= buffer[offsetof(nsmp_header_s, crc8)];
	msg->header.data_len		= buffer[offsetof(nsmp_header_s, data_len)];

	// Check CRC (before copying any data)
	if (crc_8(buffer, buffer_len) != msg->header.crc8) {
		// CRC failed - possible data corruption, request retransmit..
		return NSMP_ERR_BAD_CRC;
	}

	// Check if there is a data payload
	if (msg->header.data_len) {
		// If the user requested to write the data then copy to buffer
		if (write_data) {
			// This assumes that the user has allocated enough space for the data
			memcpy(msg->data, &buffer[sizeof(nsmp_header_s)], msg->header.data_len);
		} else {
			// Otherwise just point to the input buffer, user must copy the data
			msg->data = &buffer[sizeof(nsmp_header_s)];
		}
	}

	return NSMP_OK;
}