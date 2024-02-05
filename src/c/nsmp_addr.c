/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */
#include <stdint.h>
#include "nsmp_err.h"
#include "nsmp_addr.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Enums / Structs ------------------------- */
/* -------------------------------- Declarations ---------------------------- */

nsmp_err_e nsmp_addr_request(uint8_t address);
nsmp_err_e nsmp_addr_lease(void);

/* -------------------------------- Variables ------------------------------- */

static uint8_t s_next_addr = 0;

/* -------------------------------- Definitions ----------------------------- */

nsmp_err_e nsmp_addr_lease(void) {
}
