/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */
#include "stdio.h"

#include "nsmp.h"
#include "cobs.h"

/* -------------------------------- Defines --------------------------------- */

#define MAX_PEERS				 (32)
#define PROTOCOL_VERSION (1)
#define BROADCAST_ADDR	 (0xFF)

/* -------------------------------- Externs --------------------------------- */
/* -------------------------------- Enums ----------------------------------- */
/* -------------------------------- Structs --------------------------------- */
/* -------------------------------- Declarations ---------------------------- */

typedef unsigned int uint_t;

int get_netif(uint_t ifid, nsmp_netif_s** netif);
int process_ctl_msg(nsmp_hdr_s* hdr);

/* -------------------------------- Globals --------------------------------- */
/* -------------------------------- Locals ---------------------------------- */

static nsmp_netif_s* netif_list		 = NULL;
static uint8_t			 local_address = 0;

/* -------------------------------- Functions ------------------------------- */

int nsmp_init(nsmp_cfg_s* cfg) {
	RETURN_IF_NULL(cfg);

	/* validate the get_time function */
	// RETURN_IF_NULL(cfg->get_time_ms);
	// RETURN_ERR_IF_FALSE(0 == cfg->get_time_ms(), NSMP_ERR_BAD_TIME_FUNCTION);

	return NSMP_OK;
}

int nsmp_parse(uint8_t* source, uint32_t len) {
	RETURN_IF_NULL(source);
	RETURN_ERR_IF_FALSE(0 <= len, NSMP_ERR_BAD_LEN);

	return NSMP_OK;
}

int nsmp_send(nsmp_msg_s* msg, uint_t netif_id) {
	nsmp_netif_s*					netif;
	static cobs_enc_ctx_t ctx;
	static size_t					enc_len = 0;
	int										ret;
	size_t								len = sizeof(nsmp_hdr_s) + msg->hdr.len;

	RETURN_IF_NULL(msg);
	RETURN_IF_ERR(get_netif(netif_id, netif));

	/* initialise the cobs encoder */
	ret = cobs_encode_inc_begin(&netif->buffer, NSMP_MAX_MSG_LEN, &ctx);
	RETURN_ERR_IF_FALSE(ret == COBS_RET_SUCCESS, NSMP_ERR_COBS_ENCODE);

	/* encode the message header */
	ret = cobs_encode_inc(&ctx, &msg->hdr, sizeof(nsmp_hdr_s));
	RETURN_ERR_IF_FALSE(ret == COBS_RET_SUCCESS, NSMP_ERR_COBS_ENCODE);

	/* encode the payload (if necessary) */
	if (msg->hdr.len > 0) {
		ret = cobs_encode_inc(&ctx, msg->data, msg->hdr.len);
		RETURN_ERR_IF_FALSE(ret == COBS_RET_SUCCESS, NSMP_ERR_COBS_ENCODE);
	}

	/* finish encoding, then transmit */
	ret = cobs_encode_inc_end(&ctx, &enc_len);
	RETURN_ERR_IF_FALSE(ret == COBS_RET_SUCCESS, NSMP_ERR_COBS_ENCODE);

	RETURN_IF_ERR(netif->transmit(netif->buffer, enc_len));

	return NSMP_OK;
}

int nsmp_receive(uint8_t* buffer, size_t len, uint_t netif_id) {
	nsmp_netif_s*	 netif;
	static uint8_t dec_buffer[NSMP_MAX_MSG_LEN];
	static size_t	 dec_len;
	int						 ret;

	RETURN_IF_NULL(buffer);
	RETURN_ERR_IF_FALSE(sizeof(nsmp_hdr_s) < len, NSMP_ERR_BAD_LEN);
	RETURN_IF_ERR(get_netif(netif_id, netif));

	/* decode the message */
	ret = cobs_decode(buffer, len, dec_buffer, NSMP_MAX_MSG_LEN, &dec_len);
	RETURN_ERR_IF_FALSE(ret == COBS_RET_SUCCESS, NSMP_ERR_COBS_ENCODE);

	/* check the message header and then process it */
	nsmp_msg_s* msg = (nsmp_msg_s*)dec_buffer;

	/* check if the message has reached its destination */
	if (msg->hdr.dst == local_address || msg->hdr.dst == BROADCAST_ADDR) {
		if (msg->hdr.ctl.type == NSMP_MSG_TYPE_USER) {
			msg->data = &dec_buffer[sizeof(nsmp_hdr_s)];
			ret				= netif->receive(&msg);
		} else {
			ret = process_ctl_msg(&msg->hdr);
		}
	}
}

int nsmp_new_interface(nsmp_netif_s* netif) {
	RETURN_IF_NULL(netif);
	RETURN_IF_NULL(netif->transmit);
	RETURN_IF_NULL(netif->receive);

	if (netif_list == NULL) {
		netif->id	 = 0;
		netif_list = netif;
		return NSMP_OK;
	}

	nsmp_netif_s* tmp = netif_list;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}

	netif->id = tmp->id + 1;
	tmp->next = netif;
	return NSMP_OK;
}

int get_netif(uint_t id, nsmp_netif_s** netif) {
	RETURN_IF_NULL(netif);

	nsmp_netif_s* tmp = netif_list;
	while (tmp != NULL) {
		if (tmp->id == id) {
			*netif = tmp;
			return NSMP_OK;
		}
		tmp = tmp->next;
	}

	return NSMP_ERR_NETIF_NOT_FOUND;
}
