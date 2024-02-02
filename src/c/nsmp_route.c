/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */
#include <stdbool.h>
#include "nsmp_route.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Enums / Structs ------------------------- */

typedef struct {
	uint32_t			uuid;
	nsmp_netif_s* interface;
} route_s;

/* -------------------------------- Declarations ---------------------------- */

static bool route_exists_addr(uint8_t address);
static bool route_exists_uuid(uint32_t uuid);
static bool check_address(uint8_t address);

/* -------------------------------- Variables ------------------------------- */

static route_s s_routing_table[NSMP_MAX_NODES] = {0};

/* -------------------------------- Definitions ----------------------------- */

nsmp_err_e nsmp_route_add(uint8_t address, uint32_t uuid,
													nsmp_netif_s* interface) {
	if (interface == NULL) {
		return NSMP_ERR_NULL;
	} else if (!check_address(address)) {
		return NSMP_ERR_BAD_ADDR;
	} else if (route_exists_addr(address) || route_exists_uuid(uuid)) {
		return NSMP_ERR_ROUTE_EXISTS;
	}

	s_routing_table[address].uuid			 = uuid;
	s_routing_table[address].interface = interface;

	return NSMP_OK;
}

nsmp_err_e nsmp_route_delete(uint8_t address) {
	if (!check_address(address)) {
		return NSMP_ERR_BAD_ADDR;
	} else if (!route_exists_addr(address)) {
		return NSMP_OK;
	}

	s_routing_table[address].uuid			 = 0;
	s_routing_table[address].interface = NULL;

	return NSMP_OK;
}

static bool route_exists_addr(uint8_t address) {
	return s_routing_table[address].interface != NULL;
}

static bool route_exists_uuid(uint32_t uuid) {
	for (uint8_t i = 0; i < NSMP_MAX_NODES; ++i) {
		if (s_routing_table[i].uuid == uuid) {
			return true;
		}
	}

	return false;
}

static bool check_address(uint8_t address) {
	if (address == NSMP_BROADCAST_ADDR || address == NSMP_INVALID_ADDR) {
		return false;
	} else if (address >= NSMP_MAX_NODES) {
		return false;
	}

	return true;
}
