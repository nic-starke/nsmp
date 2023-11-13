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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/*
static const nsmp_field_s nsmp_control_bitfield[CONTROL_FIELD_NB] = {
		[CONTROL_FIELD_REQUEST_RESPONSE] = {0, 0x01},
		[CONTROL_FIELD_INCLUDES_DATA]		 = {1, 0x02},

*/

#define CONTROL_MASK_REQUEST_RESPONSE(x) ((x) & 0x01u)
#define CONTROL_MASK_HAS_DATA(x)				 (((x) & 0x01u) >> 1u)
#define CONTROL_MASK_MESSAGE_TYPE				 (((x) & 0xFCu) >> 2u)
#define ROUTING_MASK_DEVICE_TYPE(x)			 ((x) & 0x01u)
#define ROUTING_MASK_NODE_ADDRESS(x)		 (((x) & 0x02u) >> 1u)
#define ROUTING_MASK_DEVICE_ADDRESS(x)	 (((x) & 0xFCu) >> 2u)

#define NSMP_SYNC_BYTE	 0x5Au
#define NSMP_ESCAPE_BYTE 0x5Bu

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

typedef struct {
	nsmp_parser_state_e state;
	size_t							cur_len;
	size_t							exp_len;
	size_t							max_len;
	uint8_t*						data;
} nsmp_parser_ctx_s;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Variables ~~~~~~~~~~~~~~~~~~~~~~~~~ */
}
;

static nsmp_parser_ctx_s parser_ctx = {0};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Functions ~~~~~~~~~~~~~~~~~~~~~~~~ */

int nsmp_parser_reset(uint8_t* output_buffer, size_t buffer_len) {
	assert(output_buffer != NULL);
	assert(buffer_len > NSMP_HEADER_SIZE);

	parser_ctx.state	 = NSMP_PARSER_IDLE;
	parser_ctx.cur_len = 0;
	parser_ctx.exp_len = 0;
	parser_ctx.max_len = buffer_len;
	parser_ctx.data		 = output_buffer;
	return 0;
}

// stream based parser - reads one byte at a time.
int nsmp_parser_process_next(uint8_t byte) {

	// Check if we have enough space to store the next byte.
	if (parser_ctx.cur_len >= parser_ctx.max_len) {
		parser_ctx.state = NSMP_PARSER_ERR_NOMEM;
		return parser_ctx.state;
	}

	switch (parser_ctx.state) {
		case NSMP_PARSER_IDLE:
			if (byte == NSMP_SYNC_BYTE) {
				parser_ctx.cur_len = 0;
				parser_ctx.exp_len = 0;
				parser_ctx.state	 = NSMP_PARSER_HEADER;
			}
			break;

		case NSMP_PARSER_HEADER:
			parser_ctx.data[parser_ctx.cur_len++] = byte;
			if (parser_ctx.cur_len == NSMP_HEADER_SIZE) {
				bool has_data = parser_ctx.data[0] & 0x02;

				if (has_data) {
					parser_ctx.exp_len = parser_ctx.data parser_ctx.state =
							NSMP_PARSER_DATA;
				}
			}
			break;

		case NSMP_PARSER_DATA:
			parser_ctx.data[parser_ctx.cur_len++] = byte;
			if (parser_ctx.cur_len ==
					NSMP_HEADER_SIZE + parser_ctx.data[NSMP_HEADER_SIZE - 1]) {
				parser_ctx.state;
			}
			break;

		case NSMP_PARSER_COMPLETE:
			if (byte == NSMP_SYNC_BYTE) {
				parser_ctx.state = NSMP_PARSER_HEADER;
			}
			break;

		default: parser_ctx.state = NSMP_PARSER_ERR_BADSTATE; break;
	}

	return parser_ctx.state;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Functions ~~~~~~~~~~~~~~~~~~~~~~~~~ */
