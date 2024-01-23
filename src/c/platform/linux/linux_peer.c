/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */

#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

#include "nsmp_linux.h"

/* -------------------------------- Defines --------------------------------- */
/* -------------------------------- Externs --------------------------------- */

extern nsmp_netif_s nsmp_netif_linux_termios;

/* -------------------------------- Enums ----------------------------------- */
/* -------------------------------- Structs --------------------------------- */
/* -------------------------------- Declarations ---------------------------- */

static void sig_handler(int sig);
static int	process_message(nsmp_msg_s* msg);

/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */

static nsmp_netif_s* const s_netif_termios = &nsmp_netif_linux_termios;
static bool								 s_running			 = false;
static uint8_t						 s_rx_buf[NSMP_MAX_MSG_LEN] = {0};

/* -------------------------------- Functions ------------------------------- */

/**
 * @brief
 *
 * @param argc
 * @param argv Command line arguments, 1 = device name, 2 = baudrate
 * @return int
 */
int main(int argc, char** argv) {
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	const char* dev			 = argv[1];
	int					baudrate = atoi(argv[2]);

	int ret = nsmp_termios_configure(dev, baudrate);
	RETURN_IF_ERR(ret);

	ret = nsmp_netif_register(s_netif_termios);
	RETURN_IF_ERR(ret);

	ret = nsmp_discovery();
	RETURN_IF_ERR(ret);

	s_running = true;
	do {
		size_t num_rx = 0;

		ret = s_netif_termios->drv->receive(s_rx_buf, sizeof(s_rx_buf), &num_rx);
		RETURN_IF_ERR(ret);

		ret = nsmp_parse(s_rx_buf, num_rx);
		RETURN_IF_ERR(ret);
	} while (s_running);

	return EXIT_SUCCESS;
}

static int process_message(nsmp_msg_s* msg) {
	RETURN_IF_NULL(msg);

	static const char* msg_types[NSMP_MSG_TYPE_NB] = {
			[NSMP_MSG_TYPE_USER]			= "User",
			[NSMP_MSG_TYPE_DISCOVERY] = "Discovery",
	};

	if (msg->hdr.ctl.type < NSMP_MSG_TYPE_NB) {
		printf("Received %s message from %d\n", msg_types[msg->hdr.ctl.type],
					 msg->hdr.src);
	} else {
		printf("Received unknown message type from %d\n", msg->hdr.src);
	}

	return NSMP_OK;
}

static void sig_handler(int sig) {
	if (sig == SIGINT || sig == SIGTERM) {
		s_running = false;
	}
}
