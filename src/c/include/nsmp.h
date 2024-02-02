/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */
#include <stddef.h>

#include "nsmp_err.h"
#include "nsmp_msg.h"
#include "nsmp_node.h"
#include "nsmp_netif.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Enums / Structs ------------------------- */
/* -------------------------------- Declarations ---------------------------- */

nsmp_err_e nsmp_init(void);
nsmp_err_e nsmp_deinit(void);

/* -------------------------------- Variables ------------------------------- */
/* -------------------------------- Definitions ----------------------------- */
