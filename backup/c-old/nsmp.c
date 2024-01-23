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

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Extern ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef struct {
	uint8_t				addr;	 /* Local address */
	nsmp_iface_s* iface; /* Linked list of interfaces */
} nsmp_ctx_s;

/**
 * @brief Routing table.
 */
typedef struct {
	uint8_t iface_idx; /* Interface index */
	uint8_t tof;			 /* Time of flight */
} nsmp_rtab_s;

static nsmp_rtab_s rtab[NSMP_MAX_PEERS];

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Variables ~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Functions ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Functions ~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int nsmp_discovery_handler(nsmp_msg_s* msg, nsmp_iface_s* iface) {
	if (msg->hdr.ctl.reqres == NSMP_MSG_REQUEST) {

	} else {

		size_t rtab_len = sizeof(rtab);

		// Update direct connections
		iface->rtab[msg->hdr.src] = 1;
	}
}
