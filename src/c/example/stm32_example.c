/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                  Copyright (c) (2023 - 2023) Nicolaus Starke               */
/*                      https:/*github.com/nic-starke/nsmp                    */
* /
/*                         SPDX-License-Identifier: MIT                       */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stddef.h>
#include "nsmp.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define PAYLOAD_SIZE	(sizeof(my_msg_s))
#define NUM_TX_MSGS		(16)
#define NUM_RX_MSGS		(16)
#define TX_QUEUE_SIZE NSMP_QUEUE_SIZE(NUM_TX_MSGS, PAYLOAD_SIZE)
#define RX_QUEUE_SIZE NSMP_QUEUE_SIZE(NUM_RX_MSGS, PAYLOAD_SIZE)

		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Extern ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

		enum {
	MY_PROCESSOR,
	MY_CONTROLLER,
	MY_SENSOR,
	MY_DESKTOP,
	MY_SOFTWARE,
	MY_HUB,

	MY_DEVICES_NB,
};

typedef struct __attribute__((packed)) {
	uint8_t device_type;
	uint8_t message_id;
	uint8_t payload[32];
} my_msg_s;

typedef struct __attribute__((packed)) {
	uint8_t device_type;
	uint8_t fw_version;
	uint8_t hw_revision;
	uint8_t mac_addr[8];
} my_discovery_s;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Variables ~~~~~~~~~~~~~~~~~~~~~~~~~ */

static uint8_t nsmp_tx_queue[TX_QUEUE_SIZE] __attribute__((aligned(4)));
static uint8_t nsmp_rx_queue[RX_QUEUE_SIZE] __attribute__((aligned(4)));

/* The user can define their own discovery payloads, which can be used
				for various purposes, such as a simple network-service discovery system. */
static const my_discovery_s ddata = {
		.device_type = MY_SENSOR,
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Functions ~~~~~~~~~~~~~~~~~~~~~~~~ */

void uart_init(void) {
}

void uart_rx(void) {
}

void uart_tx(void) {
}

int my_payload_handler(nsmp_msg_s* m) {
	/* Do something with your payload */
	return 0;
}

int my_discovery_handler(nsmp_msg_s* m) {
	my_discovery_s* disc_data = (my_discovery_s*)m->data;

	if (disc_data->device_type == MY_SOFTWARE) {
		nsmp_msg_s reply = {
				.header.destination = m->header.source,
		};

		nsmp_send(&msg);
	}
	return 0;
}

int new_msg_cb(nsmp_msg_s* msg) {

	switch (msg->header.control.type) {
		case NSMP_MSG_TYPE_USER_MESSAGE: {
			if (msg->header.control.data == 1 && msg->data != NULL) {
				return my_payload_handler((my_msg_s*)msg->data);
			}
			break;
		}

		/* System received a discovery message, if it was a request then a response has
		already been sent by NSMP. The user can now add additional logic to handle the
		discovery message. */
		case NSMP_MSG_TYPE_CTL_DISCOVERY: {
			if (msg->header.control.response_request == 0) {
				if (msg->header.control.data == 1 && msg->data != NULL) {
					return my_discovery_handler((my_discovery_s*)msg->data);
				}
			} else {
			}
			break;
		}

		case NSMP_MSG_TYPE_CTL_REGISTER: {
			break;
		}

		case NSMP_MSG_TYPE_CTL_UNREGISTER: {
			break;
		}
	}

	/* Return 0 to indicate that the message has been handled */
}

void main(void) {

	int status = 0;

	/* Initialise the system as a NSMP node - giving it routing capability */
	status = nsmp_node_init();
	/* add error handling code */

	/* Add an comms interface to the system - UART in this case */
	nsmp_ifcfg_s uart_cfg = {
			.tx_q		 = nsmp_tx_queue,
			.rx_q		 = nsmp_rx_queue,
			.tx_size = TX_QUEUE_SIZE,
			.rx_size = RX_QUEUE_SIZE,
			.rx_cb	 = new_msg_cb,
	};

	status = nsmp_node_newif(&uart_cfg);
	/* add error handling code */

	/* Set the NSMP discovery data that will be sent out when a discovery request is received */
	status = nsmp_set_discovery_data((uint8_t*)&ddata, sizeof(ddata));
	/* add error handling code */

	/* Begin network discovery */
	status = nsmp_discover();
	/* add error handling code */

	while (1) {
		status = nsmp_update();

		if (status != 0) {
			/* User error handling code */
		}

		uint8_t payload[PAYLOAD_SIZE] = {0xAA};

		nsmp_msg_s msg = {
				.data = payload,
				.header.destination =
						{
								.device_type = DEVICE_TYPE_PEER,
								.node_addr	 = 0x00,
								.dev_addr		 = 0x00,
						},
		};

		status = nsmp_send(&msg);
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Functions ~~~~~~~~~~~~~~~~~~~~~~~~~ */
