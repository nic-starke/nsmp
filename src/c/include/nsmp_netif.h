/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */
#include "nsmp_err.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Enums / Structs ------------------------- */

typedef struct nsmp_netif_s {
	nsmp_netif_s* next; /* next interface in linked-list */
} nsmp_netif_s;

/* -------------------------------- Declarations ---------------------------- */

nsmp_err_e nsmp_netif_register(nsmp_netif_s* netif);

/* -------------------------------- Variables ------------------------------- */
/* -------------------------------- Definitions ----------------------------- */
