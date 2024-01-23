/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */

#include "unity.h"
#include "nsmp.h"
#include "nsmp_cfg.h"

/* -------------------------------- Defines --------------------------------- */

#define NB_PEERS (2)


/* -------------------------------- Externs --------------------------------- */
/* -------------------------------- Enums ----------------------------------- */
/* -------------------------------- Structs --------------------------------- */
/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */

static nsmp_cfg_s cfg[NB_PEERS];
static nsmp_netif_s netif[NB_PEERS];

/* -------------------------------- Functions ------------------------------- */

void init_client(unsigned int id, nsmp_cfg_s* cfg, nsmp_netif_s* netif) {
	TEST__PRINTF("Initialising client %d\n", netif->address);

	if (id > NB_PEERS) {
		TEST__PRINTF("ERROR: invalid peer id\n");
		TEST_ABORT();
	}
	
	int status = 0;

	status = nsmp_init(&cfg[id]);
	TEST_ASSERT_EQUAL(NSMP_OK, status);
	
	status = nsmp_add_interface(&netif[id]);
	TEST_ASSERT_EQUAL(NSMP_OK, status);
}

void discovery(unsigned int id) {
	if (id > NB_PEERS) {
		TEST__PRINTF("ERROR: invalid peer id\n");
		TEST_ABORT();
	}

	TEST__PRINTF("Peer %d: starting discovery\n", id);

	int status = 0;

	nsmp_msg_s msg = {
		.hdr = {
			.ctl = {
				.data = 0,
				.reqres = 1,
				.type = NSMP_MSG_TYPE_DISCOVERY,
			},
			.dst = BROADCAST_ADDR,
		},
	};

	uint32_t now = cfg[id].get_time_ms();

	status = nsmp_send(&msg, netif[id].id);
	TEST_ASSERT_EQUAL(NSMP_OK, status);

	/* should receive discovery replies... */
}

