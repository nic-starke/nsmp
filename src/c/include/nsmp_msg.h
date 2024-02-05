/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */
#include <stdint.h>
#include "nsmp_util.h"
#include "nsmp_netif.h"
/* -------------------------------- Defines / Externs ----------------------- */

#define NSMP_MAX_PAYLOAD_SIZE (255u) /* bytes */
#define NSMP_MIN_PAYLOAD_SIZE (127u) /* bytes */

enum {
	NSMP_REQUEST	= 0,
	NSMP_RESPONSE = 1
};

/* control bitfield */
#define OFS_REQRES							(0)
#define OFS_RETRY								(1)
#define OFS_MSG_TYPE						(2)
#define MSK_REQRES							(0x01)
#define MSK_RETRY								(0x02)
#define MSK_MSG_TYPE						(0xFC)
#define CTL_SET_REQRES(ctl, rq) BF_SET(ctl, MSK_REQRES, OFS_REQRES, rq)
#define CTL_SET_RETRY(ctl, rt)	BF_SET(ctl, MSK_RETRY, OFS_RETRY, rt)
#define CTL_SET_TYPE(ctl, mt)		BF_SET(ctl, MSK_MSG_TYPE, OFS_MSG_TYPE, mt)
#define CTL_GET_REQRES(ctl)			BF_GET(ctl, MSK_REQRES, OFS_REQRES)
#define CTL_GET_RETRY(ctl)			BF_GET(ctl, MSK_RETRY, OFS_RETRY)
#define CTL_GET_TYPE(ctl)				BF_GET(ctl, MSK_MSG_TYPE, OFS_MSG_TYPE)

/* -------------------------------- Enums / Structs ------------------------- */

enum {
	NSMP_MSG_TYPE_USER,

	NSMP_MSG_TYPE_DISCOVERY,
	NSMP_MSG_TYPE_NB,

#if (NSMP_MSG_TYPE_NB > (MSK_MSG_TYPE >> OFS_MSG_TYPE))
#error                                                                         \
		"To many message types, NSMP_MSG_TYPE_NB must fit in the control bitfield!"
#endif
};

typedef struct __attribute__((packed)) {
	uint8_t	 ctl;	 /* Control (bitfield) */
	uint8_t	 dst;	 /* Destination address */
	uint8_t	 src;	 /* Source address */
	uint8_t	 crc8; /* CRC8 checksum */
	uint16_t len;	 /* Length of the data payload */
} nsmp_hdr_s;

typedef struct {
	nsmp_hdr_s		hdr;	 /* NSMP header */
	nsmp_netif_s* netif; /* Network interface */
	uint8_t*			data;	 /* Pointer to the data payload */
} nsmp_msg_s;

/* -------------------------------- Declarations ---------------------------- */

nsmp_err_e nsmp_msg_send(nsmp_msg_s* msg);

/* broadcast on all interfaces */
nsmp_err_e nsmp_msg_broadcast_all(nsmp_msg_s* msg);

/* broadcast on all "other" interfaces (all except the interface set
within the msg header) */
nsmp_err_e nsmp_msg_broadcast_other(nsmp_msg_s* msg);

nsmp_err_e nsmp_msg_add_payload(nsmp_msg_s* msg, uint8_t* payload, size_t len);

/* -------------------------------- Variables ------------------------------- */
/* -------------------------------- Definitions ----------------------------- */
