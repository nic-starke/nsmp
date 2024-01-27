/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */
#include "nsmp.h"
#include "nsmp_netif.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Enums / Structs ------------------------- */
/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Variables ------------------------------- */

/* pointer to first interface */
static nsmp_netif_s* s_if = NULL;

/* -------------------------------- Definitions ----------------------------- */

nsmp_err_e nsmp_netif_register(nsmp_netif_s* netif) {
	if (netif == NULL) {
		return NSMP_ERR_NULL;
	}

	if (s_if == NULL) {
		s_if = netif;
	} else {
		nsmp_netif_s* if_iter = s_if;
		while (if_iter->next != NULL) {
			if_iter = if_iter->next;
		}
		if_iter->next = netif;
	}

	return NSMP_OK;
}