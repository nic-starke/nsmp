/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*                  Copyright (c) (2023 - 2023) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#pragma once
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include <stddef.h>
#include <stdint.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define NSMP_MAX_PEERS				(32)
#define NSMP_PROTOCOL_VERSION (0x01)

/* Macro to calculate NSMP queue length based on:
 	 x - number of messages in the queue
	 p - the maximum payload length (user-defined)

	 The queue length is calculated as follows:
	 number of messages * (nsmp header + user payload) + (2 * number of messages)

	 The last term is used to store the size of the payload in each message.
	 
*/
#define NSMP_QUEUE_LEN(m, p) ((m) * (p + sizeof(nsmp_hdr_s)) + (2 * m))

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Extern ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

typedef enum {
	NSMP_MSG_TYPE_USER_MESSAGE,

	/* Discovery/Routing */
	NSMP_MSG_TYPE_CTL_DISCOVERY,
	NSMP_MSG_TYPE_CTL_REGISTER,
	NSMP_MSG_TYPE_CTL_UNREGISTER,

	/* ---- INTERNAL MESSAGES ---- */

	/* Flow control */
	NSMP_MSG_TYPE_CTL_ACK,
	NSMP_MSG_TYPE_CTL_PLS_RETRY,
	NSMP_MSG_TYPE_CTL_SLOWDOWN,

	NSMP_MSG_TYPE_NB,
} nsmp_msg_type_e;

enum {
	NSMP_MSG_RESPONSE = 0,
	NSMP_MSG_REQUEST	= 1,
};

typedef struct {
	uint8_t					data	 : 1; /* 0 = no data, 1 = data */
	uint8_t					reqres : 1; /* 0 = response, 1 = request */
	uint8_t					retry	 : 1; /* 0 = first, 1 = retransmit */
	nsmp_msg_type_e type	 : 5;
} nsmp_ctrl_s;

typedef struct __attribute__((packed)) {
	nsmp_ctrl_s ctl;
	uint8_t			dst;
	uint8_t			src;
	uint8_t			crc8;
} nsmp_hdr_s;

typedef struct __attribute__((packed)) {
	nsmp_hdr_s hdr;
	uint16_t	 len; /* Length of the data payload */
	uint8_t*	 data;
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

	/* User function get system time */
	uint32_t (*get_time_ms)(void);

} nsmp_cfg_s;

/**
 * @brief A structure to hold the configuration of an NSMP interface.
 * This structure must be statically allocated by the user, and
 * passed to the nsmp_newif() function to register the interface.
 * 

 * 
 */
typedef struct nsmp_iface_s {
	// private:
	struct nsmp_iface_s* next;

	// public:
	uint8_t	 uuid[8];
	uint8_t* tx_q;
	uint8_t* rx_q;
	size_t	 tx_len;
	size_t	 rx_len;
	int (*rx_cb)(nsmp_msg_s* msg);
	int (*tx_cb)(nsmp_msg_s* msg);
} nsmp_iface_s;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Variables ~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Variables ~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Global Functions ~~~~~~~~~~~~~~~~~~~~~~~~ */

/**
 * @brief Initialise the device as an NSMP peer.
 * 
 * @return int #TODO
 */
int nsmp_peer_init(void);

/**
 * @brief Initialise the device as an NSMP node.
 * 
 * @return int #TODO
 */
int nsmp_node_init(void);

/**
 * @brief Register a new interface with the NSMP node.
 * 
 * @param ifcfg Pointer to interface configuration structure, must be
 * statically allocated.
 * 
 * @return int #TODO
 */
int nsmp_node_newif(nsmp_iface_s* ifcfg);

/**
 * @brief Parse an incoming byte stream to extract NSMP messages.
 * 
 * @param inbuf Pointer to byte stream.
 * @param inlen Number of bytes to parse.
 * @return int #TODO
 */
int nsmp_parse(uint8_t* inbuf, size_t inlen);

/**
 * @brief Performs NSMP network discovery.
 * The user can process each response individually.
 * @return int #TODO
 */
int nsmp_discover(void);

/**
 * @brief Add a message to the outgoing queue.
 * Will be transmitted on the next NSMP update() call, and when the 
 * appropriate interface is ready.
 * 
 * This function returns immediately if there is no space in the queue.
 * See nsmp_send_wait() for a blocking version.
 * 
 * @param msg Pointer to nsmp message.
 * @return int #TODO
 */
int nsmp_send(nsmp_msg_s* msg);

/**
 * @brief Add a message to the outgoing queue.
 * Will be transmitted on the next NSMP update() call, and when the
 * appropriate interface is ready.
 * 
 * This function blocks for the specified number of milliseconds if there is no
 * space in the queue.
 * 
 * @param msg Pointer to nsmp message.
 * @param waitms Number of milliseconds to wait for space in the queue.
 * @return int #TODO
 */
int nsmp_send_wait(nsmp_msg_s* msg, uint32_t waitms);

/**
 * @brief Add a user-payload to an NSMP message.
 * This is the only way to add data to an NSMP message, do not attempt
 * to modify the nsmp msg data structure directly.
 * 
 * @param msg Pointer to nsmp message.
 * @param payload Pointer to user payload.
 * @param len Length of user payload.
 * @return int #TODO
 */
int nsmp_add_data(nsmp_msg_s* msg, uint8_t* payload, size_t len);

/**
 * @brief Adds a user-payload to a discovery response message, before
 * it is transmitted to other nodes/peers.
 * 
 * @param payload Pointer to user payload.
 * @param len Length of user payload.
 * @return int #TODO
 */
int nsmp_set_discovery_payload(uint8_t* payload, size_t len);

/**
 * @brief Get the length of an NSMP message (including payload).
 * 
 * @param msg Pointer to nsmp message.
 * @return size_t Length of nsmp message.
 */
size_t nsmp_message_len(nsmp_msg_s* msg);

int nsmp_rcv(nsmp_msg_s* msg);
int nsmp_rcv_wait(nsmp_msg_s* msg, uint32_t waitms);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Local Functions ~~~~~~~~~~~~~~~~~~~~~~~~~ */
