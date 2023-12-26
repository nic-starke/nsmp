/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                  Copyright (c) (2023 - 2023) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#pragma once
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stdint.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define NSMP_PROTOCOL_VERSION 0x01

#define NSMP_QUEUE_SIZE(x, p) ((x) * (p + sizeof(nsmp_hdr_s)) + (2 * x))

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Extern ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef enum {
	NSMP_MSG_TYPE_USER_MESSAGE,

	// Discovery/Routing
	NSMP_MSG_TYPE_CTL_DISCOVERY,
	NSMP_MSG_TYPE_CTL_REGISTER,
	NSMP_MSG_TYPE_CTL_UNREGISTER,

	// ---- INTERNAL MESSAGES ----

	// Flow control
	NSMP_MSG_TYPE_CTL_ACK,
	NSMP_MSG_TYPE_CTL_PLS_RETRY,
	NSMP_MSG_TYPE_CTL_SLOWDOWN,

	NSMP_MSG_TYPE_NB,
} nsmp_msg_type_e;

typedef enum {
	DEVICE_TYPE_PEER	 = 0x00,
	DEVICE_TYPE_NODE	 = 0x01,
} nsmp_device_type_e;

typedef struct {
	uint8_t					data						 : 1; // 0 = no data, 1 = data
	uint8_t					response_request : 1; // 0 = response, 1 = request
	uint8_t					retransmit			 : 1; // 0 = first, 1 = retransmit
	nsmp_msg_type_e type						 : 5;
} nsmp_ctrl_s;

typedef struct {
	uint8_t device_type : 1; // 0 = device, 1 = node
	uint8_t node_addr		: 3;
	uint8_t dev_addr		: 4;
} nsmp_addr_s;

typedef struct __attribute__((packed)) {
	nsmp_ctrl_s control;
	nsmp_addr_s destination;
	nsmp_addr_s source;
	uint8_t			crc8;
} nsmp_hdr_s;

typedef struct {
	nsmp_hdr_s		header;
	uint8_t*			data;
} nsmp_msg_s;

enum {
	NSMP_ERR_BAD_ARG = -1,
	NSMP_ERR_BAD_CRC = -2,
	NSMP_ERR_BAD_LEN = -3,
	NSMP_ERR_BAD_VER = -4,

	NSMP_OK = 0,
};

typedef struct {
	uint8_t uuid[8];
} nsmp_cfg_s;

typedef struct {
	uint8_t* tx_q;
	uint8_t* rx_q;
	size_t	 tx_size;
	size_t	 rx_size;
	int (*rx_cb)(nsmp_msg_s* msg);
} nsmp_ifcfg_s;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Variables ~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Functions ~~~~~~~~~~~~~~~~~~~~~~~~ */

int nsmp_peer_init(void); // Initialise the device as a peer

int nsmp_node_init(void);									// Initialise the device as a node
int nsmp_node_newif(nsmp_ifcfg_s* ifcfg); // Register a new interface

int nsmp_set_discovery_data(uint8_t* data, size_t len);

int nsmp_parser_init(void); // Initialise the parser
int nsmp_parse(uint8_t* buffer, size_t len);

int nsmp_discover(void);

// Send a message
int nsmp_send(nsmp_msg_s* msg);
int nsmp_send_wait(nsmp_msg_s* msg, uint32_t waitms);
int nsmp_add_data(nsmp_msg_s* msg, uint8_t* payload, size_t len);

// Check for a message
int nsmp_rcv(nsmp_msg_s* msg);
int nsmp_rcv_wait(nsmp_msg_s* msg, uint32_t waitms);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Functions ~~~~~~~~~~~~~~~~~~~~~~~~~ */
