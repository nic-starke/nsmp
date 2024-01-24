/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */

#include "nsmp_linux.h"

/* -------------------------------- Defines --------------------------------- */

#define NB_INTERFACES (2)

/* -------------------------------- Externs --------------------------------- */
/* -------------------------------- Enums ----------------------------------- */
/* -------------------------------- Structs --------------------------------- */
/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */

static nsmp_linux_netif_ctx_s s_ctx[NB_INTERFACES];
static nsmp_netif_s						s_netif[NB_INTERFACES];

/* -------------------------------- Functions ------------------------------- */

void setUp(void) {
	TEST__PRINTF("Setting up\n");

	for (unsigned int i = 0; i < NB_INTERFACES; i++) {
		s_netif[i].transmit = nsmp_linux_netif_transmit;

		const char* dev = "/dev/ttyUSB";
		snprintf(s_ctx[i].dev, sizeof(s_ctx[i].dev), "%s%d", dev, i);
		s_ctx[i].baud = 115200;

		int status = nsmp_linux_netif_init(&s_ctx[i]);
		TEST_ASSERT_EQUAL(NSMP_OK, status);
	}
}

void test_main(void) {
	TEST__PRINTF("Starting test\n");

	nsmp_send();
}
