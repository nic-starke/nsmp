/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */
#include <stdint.h>
#include "nsmp_err.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Types / Enums / Structs ----------------- */

#define NSMP_UID_LEN 16

/* -------------------------------- Declarations ---------------------------- */

nsmp_err_e nsmp_uid_init(uint64_t seed);
nsmp_err_e nsmp_uid_generate(char uid[NSMP_UID_LEN]);

/* -------------------------------- Variables ------------------------------- */
/* -------------------------------- Definitions ----------------------------- */
