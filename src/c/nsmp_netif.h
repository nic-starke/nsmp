/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */

#include "stddef.h"
#include "stdint.h"

/* -------------------------------- Defines --------------------------------- */
/* -------------------------------- Externs --------------------------------- */
/* -------------------------------- Enums ----------------------------------- */
/* -------------------------------- Structs --------------------------------- */

struct nsmp_netif_ctx_s;
struct nsmp_netif_drv_s;

typedef struct {
	int (*init)(void);
	int (*open)(void);
	int (*close)(void);
	int (*transmit)(uint8_t* buffer, size_t num_tx);
	int (*receive)(uint8_t* buffer, size_t buf_len, size_t* num_rx);
} nsmp_netif_drv_s;

typedef struct {
	uint8_t*		 tx_buf;
	uint8_t*		 rx_buf;
	const size_t tx_len;
	const size_t rx_len;
} nsmp_netif_ctx_s;

typedef struct nsmp_netif_s {
	uint8_t									id;
	nsmp_netif_drv_s* const drv;
	nsmp_netif_ctx_s* const ctx;
	nsmp_netif_s*						next;
} nsmp_netif_s;

/* -------------------------------- Declarations ---------------------------- */

int nsmp_netif_register(nsmp_netif_s* netif);

/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */
/* -------------------------------- Functions ------------------------------- */
