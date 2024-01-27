/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */

#include "inttypes.h"
#include "stddef.h"

#include "nsmp_err.h"
#include "nsmp_cfg.h"
#include "nsmp_netif.h"

/* -------------------------------- Defines --------------------------------- */

/* Macro to calculate NSMP queue length based on:
	 n - number of messages in the queue
	 p - to maximum payload length (user-defined)

	 The queue length is calculated as follows:
	 number of messages * (nsmp header + user payload) + (2 * number of messages)

	 The last term (2*n) is used to store the size of the payload in each message,
	 which is a 16-bit value.
*/
#define NSMP_QUEUE_LEN(n, p) ((n) * (p + sizeof(nsmp_hdr_s)) + (n))
#define NSMP_MAX_MSG_LEN		 (sizeof(nsmp_hdr_s) + NSMP_MAX_PAYLOAD_LEN)

/* -------------------------------- Externs --------------------------------- */
/* -------------------------------- Enums ----------------------------------- */

enum {
	NSMP_MSG_TYPE_USER,

	NSMP_MSG_TYPE_DISCOVERY,
	NSMP_MSG_TYPE_NB,

#if (NSMP_MSG_TYPE_NB >= 64)
#error "NSMP_MSG_TYPE_NB too large!"
#endif
};

enum {
	NSMP_PEER_STATE_UNINITIALIZED,
	NSMP_PEER_STATE_READY,
	NSMP_PEER_STATE_ACTIVE,
	NSMP_PEER_STATE_ERROR,

	NSMP_PEER_STATE_NB,
};

/* -------------------------------- Structs --------------------------------- */

typedef struct __attribute__((packed)) {
	uint8_t reqres : 1; /* 0 = response, 	1 = request */
	uint8_t retry	 : 1; /* 0 = initial, 	1 = retransmit */
	uint8_t type	 : 6; /* message type */
} nsmp_ctrl_s;

typedef struct __attribute__((packed)) {
	nsmp_ctrl_s ctl;	/* Control bits */
	uint8_t			dst;	/* Destination address */
	uint8_t			src;	/* Source address */
	uint8_t			crc8; /* CRC8 checksum */
	uint16_t		len;	/* Length of the data payload */
} nsmp_hdr_s;

typedef struct __attribute__((packed)) {
	nsmp_hdr_s hdr;	 /* NSMP header */
	uint8_t*	 data; /* Pointer to the data payload */
} nsmp_msg_s;

/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */
/* -------------------------------- Functions ------------------------------- */
