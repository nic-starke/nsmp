/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */

#include "unity.h"
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
	TEST_PRINTF("Setting up\n");

	for (unsigned int i = 0; i < NB_INTERFACES; i++) {
		const char* dev = "/dev/ttyUSB";
		snprintf(s_ctx[i].dev, sizeof(s_ctx[i].dev), "%s%d", dev, i);
		s_ctx[i].baud = 115200;

		int status = nsmp_linux_netif_init(&s_ctx[i]);
		TEST_ASSERT_EQUAL(NSMP_OK, status);
	}
}

void tearDown(void) {
	TEST_PRINTF("Tearing down\n");

	for (unsigned int i = 0; i < NB_INTERFACES; i++) {
		int status = nsmp_linux_netif_stop(&s_ctx[i]);
		TEST_ASSERT_EQUAL(NSMP_OK, status);
	}
}

void test_discovery(void) {
	nsmp_netif_discovery(&s_netif[0]);
}

void test_send_broadcast(void) {
	nsmp_msg_s msg			 = {0};
	const char payload[] = "Hello World!";
	int				 ret = nsmp_add_payload(&msg, (uint8_t*)payload, sizeof(payload));
	TEST_ASSERT_EQUAL(NSMP_OK, ret);

	ret = nsmp_broadcast(&msg, &s_netif[0]);
}
