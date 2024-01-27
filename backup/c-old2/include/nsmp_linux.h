/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */

#include "nsmp.h"

/* -------------------------------- Defines --------------------------------- */
/* -------------------------------- Externs --------------------------------- */
/* -------------------------------- Enums ----------------------------------- */
/* -------------------------------- Structs --------------------------------- */

typedef struct {
	int					 state;
	int					 fd;
	unsigned int baud;
	const char*	 dev;
	uint8_t			 tx_buf[NSMP_MAX_MSG_LEN];
	uint8_t			 rx_buf[NSMP_MAX_MSG_LEN];
} nsmp_linux_netif_ctx_s;

/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */
/* -------------------------------- Functions ------------------------------- */

int nsmp_linux_netif_init(nsmp_linux_netif_ctx_s* ctx);
int nsmp_linux_netif_stop(nsmp_linux_netif_ctx_s* ctx);
int nsmp_linux_netif_update(nsmp_linux_netif_ctx_s* ctx);
int nsmp_linux_netif_transmit(nsmp_linux_netif_ctx_s* ctx, uint8_t* buffer,
															size_t num_tx);
int nsmp_linux_netif_receive(nsmp_linux_netif_ctx_s* ctx, uint8_t* buffer,
														 size_t buf_len, size_t* num_rx);
