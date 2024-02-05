/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */
#include <stdbool.h>
#include <stddef.h>
#include "nsmp_err.h"
#include "nsmp_route.h"
#include "nsmp_netif.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Enums / Structs ------------------------- */

typedef struct {
	nsmp_netif_s* interface;
} route_s;

/* -------------------------------- Declarations ---------------------------- */

static bool route_exists_addr(nsmp_addr_t address);
static bool check_address(nsmp_addr_t address);

/* -------------------------------- Variables ------------------------------- */

static route_s s_routing_table[NSMP_MAX_NODES] = {0};

/* -------------------------------- Definitions ----------------------------- */

nsmp_err_e nsmp_route_add(nsmp_addr_t address, nsmp_netif_s* interface) {
	if (interface == NULL) {
		return NSMP_ERR_NULL;
	} else if (!check_address(address)) {
		return NSMP_ERR_BAD_ADDR;
	} else if (route_exists_addr(address)) {
		return NSMP_ERR_ROUTE_EXISTS;
	}

	s_routing_table[address].interface = interface;

	return NSMP_OK;
}

nsmp_err_e nsmp_route_delete(nsmp_addr_t address) {
	if (!check_address(address)) {
		return NSMP_ERR_BAD_ADDR;
	} else if (!route_exists_addr(address)) {
		return NSMP_OK;
	}

	s_routing_table[address].interface = NULL;

	return NSMP_OK;
}

static bool route_exists_addr(nsmp_addr_t address) {
	bool valid = (s_routing_table[address].interface != NULL);
	return valid;
}

static bool check_address(nsmp_addr_t address) {
	if (address == NSMP_BROADCAST_ADDR || address == NSMP_INVALID_ADDR) {
		return false;
	} else if (address >= NSMP_MAX_NODES) {
		return false;
	}

	return true;
}
