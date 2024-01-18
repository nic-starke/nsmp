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
	NSMP_MSG_TYPE_NB,
};

/* -------------------------------- Structs --------------------------------- */

typedef struct __attribute__((packed)) {
	uint8_t data	 : 1; /* 0 = no data, 	1 = data */
	uint8_t reqres : 1; /* 0 = response, 	1 = request */
	uint8_t retry	 : 1; /* 0 = initial, 	1 = retransmit */
	uint8_t type	 : 5; /* message type */
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

typedef struct {
	// uint32_t (*get_time_ms)(void); /* User function - get system time */
} nsmp_cfg_s;

typedef struct nsmp_netif_s {
	struct nsmp_netif_s* next;				/* Pointer to next interface */
	unsigned int				 id;					/* Interface ID */
	uint8_t							 addr;				/* NSMP address */
	uint8_t buffer[NSMP_MAX_MSG_LEN]; /* Buffer for outgoing message */

	/* RX callback function - user must copy the message before returning */
	int (*receive)(nsmp_msg_s* msg);

	/* TX function - will be called when a nsmp_send is requested for the
	specified interface */
	int (*transmit)(uint8_t* buffer, size_t len); /* Low-level TX for interface */
} nsmp_netif_s;

/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */
/* -------------------------------- Functions ------------------------------- */
