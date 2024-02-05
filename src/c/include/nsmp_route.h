/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */
#include "nsmp_addr.h"
#include "nsmp_err.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Enums / Structs ------------------------- */
/* -------------------------------- Declarations ---------------------------- */

nsmp_err_e nsmp_route_add(uint8_t address, nsmp_netif_s* interface);
nsmp_err_e nsmp_route_delete(uint8_t address);

/* -------------------------------- Variables ------------------------------- */
/* -------------------------------- Definitions ----------------------------- */
