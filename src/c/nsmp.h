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

/* -------------------------------- Defines --------------------------------- */
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
	nsmp_ctrl_s ctl;
	uint8_t			dst;
	uint8_t			src;
	uint8_t			crc8;
} nsmp_hdr_s;

typedef struct __attribute__((packed)) {
	nsmp_hdr_s hdr;
	uint16_t	 len; /* Length of the data payload */
	uint8_t*	 data;
} nsmp_msg_s;

typedef struct {
	uint32_t (*get_time_ms)(void); /* function pointer - get system time */
} nsmp_cfg_s;

/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */
/* -------------------------------- Functions ------------------------------- */
