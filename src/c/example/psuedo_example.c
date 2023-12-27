/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                  Copyright (c) (2023 - 2023) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/* 																																						*/
/*                         SPDX-License-Identifier: MIT                       */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stddef.h>
#include "nsmp.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define PAYLOAD_LEN	 (sizeof(msg_s))
#define NUM_TX_MSGS	 (16)
#define NUM_RX_MSGS	 (16)
#define TX_QUEUE_LEN NSMP_QUEUE_LEN(NUM_TX_MSGS, PAYLOAD_LEN)
#define RX_QUEUE_LEN NSMP_QUEUE_LEN(NUM_RX_MSGS, PAYLOAD_LEN)

#define UART_FIFO_LEN (32)

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

typedef struct __attribute__((packed, aligned(4))) {
	uint8_t device_type;
	uint8_t fw_version;
	uint8_t hw_revision;
	uint8_t pad;
	uint8_t mac_addr[8];
} device_info_s;

typedef struct __attribute__((packed, aligned(4))) {
	device_info_s device;
	uint8_t				id;
	uint8_t				data[32];
} msg_s;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void uart_init(void);
static void uart_rx(void);
static void uart_tx(uint8_t data, size_t len);
static int	discovery_handler(nsmp_msg_s* msg);
static int	msg_handler(nsmp_msg_s* msg);
static int	rx_cb(nsmp_msg_s* msg);
static int	tx_cb_uart(nsmp_msg_s* msg);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Variables ~~~~~~~~~~~~~~~~~~~~~~~~~ */

static uint8_t nsmp_tx_queue[TX_QUEUE_LEN] __attribute__((aligned(4)));
static uint8_t nsmp_rx_queue[RX_QUEUE_LEN] __attribute__((aligned(4)));

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Functions ~~~~~~~~~~~~~~~~~~~~~~~~ */

void main(void) {

	int status = 0;

	/* Initialise interfaces/peripherals/etc */
	uart_init();

	/* Initialise the system as a NSMP node - giving it routing capability */
	status = nsmp_node_init();
	/* add error handling code */

	/* Add a comms interface to the system - UART in this case */
	static nsmp_iface_s uart_if = {
			.tx_q		= nsmp_tx_queue,
			.rx_q		= nsmp_rx_queue,
			.tx_len = TX_QUEUE_LEN,
			.rx_len = RX_QUEUE_LEN,
			.rx_cb	= rx_cb,
			.tx_cb	= tx_cb_uart,
	};

	status = nsmp_node_newif(&uart_if);
	/* add error handling code */

	/* Begin network discovery */
	status = nsmp_discover();
	/* add error handling code */

	while (1) {
		status = nsmp_update();
		/* add error handling code */
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Functions ~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* TODO */
static void uart_init(void) {
}

/* TODO */
static void uart_rx(void) {
	uint8_t buffer[UART_FIFO_LEN];

	/* NSMP uses a streaming parser - the number of input bytes is arbitrary. 
	 * Once a complete message has been received it is added to the receive
	 * queue and the rx_cb function is called. The parser automatically
	 * resets between messages.
	*/
	nsmp_parse(buffer, UART_FIFO_LEN);
}

/* TODO */
static void uart_tx(uint8_t data, size_t len) {
	for (size_t i = 0; i < len; i++) {}
}

int rx_cb(nsmp_msg_s* msg) {
	int status = 0;

	switch (msg->hdr.ctl.type) {
		case NSMP_MSG_TYPE_USER_MESSAGE: {
			status = msg_handler(msg);
			/* error handling... */
			break;
		}

		/* NSMP will reply to all discovery requests automatically.
		 * The user can add their own payload to the response message, * thus enabling 
		 * some form of service discovery. Note - the response will be transmitted after 
		 * the message handler callback as returned. 
		 */
		case NSMP_MSG_TYPE_CTL_DISCOVERY: {
			status = discovery_handler(msg);
			/* error handling... */
			break;
		}
	}

	/* Return 0 to indicate that the message has been handled */

	return 0;
}

static tx_cb_uart(nsmp_msg_s* msg) {
	uart_tx((uint8_t*)msg, nsmp_message_len(msg));
	return 0;
}

static int discovery_handler(nsmp_msg_s* msg) {
	int status = 0;

	if (msg->hdr.ctl.reqres == NSMP_MSG_REQUEST) {
		static const device_info_s d = {
				.device_type = MY_PROCESSOR,
		};

		status = nsmp_set_discovery_payload((uint8_t*)&d, sizeof(d));
		/* error handling... */
	} else {
		device_info_s* d = (device_info_s*)msg->data;
		/* do something... */

		return status;
	}
}

static int msg_handler(nsmp_msg_s* msg) {
	int status = 0;

	msg_s* m = (msg_s*)msg->data;
	/* do something... */

	return status;
}