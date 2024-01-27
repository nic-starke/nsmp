/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */

#include "stddef.h"
#include "stdint.h"

#include "nsmp.h"

/* -------------------------------- Defines --------------------------------- */
/* -------------------------------- Externs --------------------------------- */
/* -------------------------------- Enums ----------------------------------- */
/* -------------------------------- Structs --------------------------------- */

typedef struct nsmp_netif_s {
	uint8_t				id;
	nsmp_netif_s* next;
	uint8_t				address;
	uint8_t				tx_buf[NSMP_MAX_MSG_LEN];
	uint8_t				rx_buf[NSMP_MAX_MSG_LEN];
	int (*transmit)(void* ctx, uint8_t* buffer, size_t num_tx);
} nsmp_netif_s;

/* -------------------------------- Declarations ---------------------------- */

int nsmp_netif_register(nsmp_netif_s* netif);

/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */
/* -------------------------------- Functions ------------------------------- */
