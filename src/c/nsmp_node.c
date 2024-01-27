/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */
#include "nsmp.h"
#include "nsmp_netif.h"
/* -------------------------------- Defines / Externs ----------------------- */

/* -------------------------------- Enums / Structs ------------------------- */

typedef struct {
	uint8_t				address;
	nsmp_netif_s* interface;
	unsigned int	hops;
} route_s;

/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Variables ------------------------------- */

static route_s s_routing_table[NSMP_MAX_NODES];

/* -------------------------------- Definitions ----------------------------- */
