/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */

/*
	How does NSMP discovery work?

	When a new node is connected to the network, it needs an address, but
	it also needs to generate a routing table so it can transmit messages.

	NSMP will transmit a discovery request on each network interface (when it
	is registered for the first time). The request is sent as a broadcast message,
	therefore reaching all nodes. Every node that receives a discovery request
	must:

		- Retransmit the broadcast on all other interfaces.
		- Transmit a discovery response.

	If the local node is also new to the network (has no address yet) then it
	should process any discovery responses it receives - and update its
	local routing table.

	If a discovery REQUEST is received:
	- Reply with a discovery RESPONSE.
	- Retransmit the request on all OTHER network interfaces.

	If a discovery RESPONSE is received:
	- Update the routing table with the source address and the network interface
		used to receive the message.
	- Retransmit the response back on the same network interface it was received
	on, only if we have a cache of the UID from the original request.

	Why is this important - it stops infinite retransmission loops.

*/

/* -------------------------------- Includes -------------------------------- */
#include <stddef.h>
#include <stdbool.h>
#include "nsmp_discovery.h"
#include "nsmp_route.h"
#include "nsmp_msg.h"
#include "nsmp_uid.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Enums / Structs ------------------------- */

enum {
	DISCOVERY_STATE_IDLE,
	DISCOVERY_STATE_REQUEST,
	DISCOVERY_STATE_WAIT,

	DISCOVERY_STATE_NB,
};

/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Variables ------------------------------- */

static unsigned int s_state			 = DISCOVERY_STATE_IDLE;
static uint32_t			s_start_time = 0;
static uint32_t			s_timeout		 = 0;

/* -------------------------------- Definitions ----------------------------- */

nsmp_err_e nsmp_discover(void) {

	switch (s_state) {

		case DISCOVERY_STATE_IDLE: {
			s_state = DISCOVERY_STATE_REQUEST;
		} /* fall-through */

		case DISCOVERY_STATE_REQUEST: {
			nsmp_msg_s msg;
			CTL_SET_TYPE(msg.hdr.ctl, NSMP_MSG_TYPE_DISCOVERY);
			CTL_SET_REQRES(msg.hdr.ctl, NSMP_REQUEST);

			/* add a uid for this broadcast message */
			char uid[NSMP_UID_LEN];
			nsmp_uid_generate(uid);

			int ret = nsmp_msg_add_payload(&msg, uid, NSMP_UID_LEN);
			if (ret != NSMP_OK) {
				s_state = DISCOVERY_STATE_IDLE;
				return ret;
			}

			ret = nsmp_msg_broadcast(&msg);
			if (ret != NSMP_OK) {
				s_state = DISCOVERY_STATE_IDLE;
				return ret;
			} else {
				s_state			 = DISCOVERY_STATE_WAIT;
				s_start_time = nsmp_get_time();
			}

			break;
		}

		case DISCOVERY_STATE_WAIT: {
			uint32_t current_time = nsmp_get_time();

			if (current_time > s_start_time + s_timeout) {
				s_state = DISCOVERY_STATE_IDLE;
				return NSMP_OK;
			} else {
				return NSMP_BUSY;
			}
			break;
		}

		default: return NSMP_ERR_GURU_MEDITATION;
	}

	return NSMP_ERR_GURU_MEDITATION;
}

nsmp_err_e nsmp_discovery_handler(nsmp_msg_s* msg) {
	static char					 uid_cache[4][NSMP_UID_LEN] = {0};
	static nsmp_netif_s* netif[4]										= {0};
	static unsigned int	 last_uid										= 0;
	static int					 seen_uid										= -1;
	int									 ret												= NSMP_OK;

	/* cache the uid and the network interface */
	for (unsigned int i = 0; i < 4; i++) {
		if (memcmp(uid_cache[i], msg->data, NSMP_UID_LEN) == 0) {
			seen_uid = i;
			break;
		}

		memcpy(uid_cache[last_uid], msg->data, NSMP_UID_LEN);
		netif[last_uid] = msg->netif;
		last_uid				= (last_uid + 1) % 4;
	}

	if (CTL_GET_REQRES(msg->hdr.ctl) == NSMP_REQUEST) {
		if (seen_uid != -1) {
			return NSMP_OK;
		}

		nsmp_msg_s reply = {0};
		CTL_SET_TYPE(reply.hdr.ctl, NSMP_MSG_TYPE_DISCOVERY);
		CTL_SET_REQRES(reply.hdr.ctl, NSMP_RESPONSE);
		reply.hdr.src = nsmp_get_address();
		reply.netif		= msg->netif;

		/* the remote address will probably be INVALID_ADDR, but set it anyway */
		reply.hdr.dst = msg->hdr.src;

		ret = nsmp_msg_send(&reply);
		if (ret != NSMP_OK) {
			return ret; /* TODO - handle the error, do not return it..*/
		}

		/* broadcast message */
		nsmp_msg_broadcast_other(&msg);

	} else if (CTL_GET_REQRES(msg->hdr.ctl) == NSMP_RESPONSE) {

		nsmp_route_add(msg->hdr.src, msg->netif);
		/* TODO - what can we do if an error is returned here? */

		if (seen_uid != -1) {
			msg->netif = netif[seen_uid];
			ret				 = nsmp_msg_send(msg);
			/* TODO - handle the error, do not return it..*/
		} else {
			ret = nsmp_msg_broadcast_other(msg);
			/* TODO - handle the error, do not return it..*/
		}
	}

	return NSMP_OK;
}
