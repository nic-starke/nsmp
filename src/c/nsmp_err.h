/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */

#pragma once

/*
	Two different print functions are defined for tracing errors onto stderr.
	The first prints verbose information (if compiled with DEBUG).
	The second prints an error code - this can be then be interpreted later,
	 effectively reducing the size of the release binaries (if compiled with
	NDEBUG). Its not quite defmt from rust but it will do for now.
*/

/* -------------------------------- Includes -------------------------------- */
#include "stdio.h"
/* -------------------------------- Defines --------------------------------- */

#ifdef ENABLE_ERROR_PRINT

#ifdef DEBUG
#define printerr(e) fprintf(stderr, "%s:%d: %d\n", __FILE__, __LINE__, e)
#endif

#ifdef NDEBUG
#define printerr(e) fprintf(stderr, "NSMP_ERR: %d\n", e)
#endif

#else
#define printerr(e)
#endif

#define RETURN_IF_ERR(b, e)                                                    \
	if (!(b)) {                                                                  \
		printerr(e);                                                               \
		return e;                                                                  \
	}

#define RETURN_IF_NULL(x)                                                      \
	if (NULL == (x))                                                             \
	return NSMP_ERR_NULL_PTR

#define ERROR_DEFINITIONS                                                      \
	E(NSMP_OK, "OK")                                                             \
                                                                               \
	E(NSMP_ERR_BAD_ARG, "Bad parameter/argument")                                \
	E(NSMP_ERR_BAD_CRC, "Bad CRC value")                                         \
	E(NSMP_ERR_BAD_LEN, "Bad message length")                                    \
	E(NSMP_ERR_BAD_VER, "Bad protocol version")                                  \
	E(NSMP_ERR_BAD_CFG, "Bad configuration")                                     \
	E(NSMP_ERR_BAD_TIME_FUNCTION, "Bad time function")                           \
	E(NSMP_ERR_NULL_PTR, "Null pointer")

/* -------------------------------- Externs --------------------------------- */
/* -------------------------------- Enums ----------------------------------- */

#define E(code, str) code,
typedef enum {
	ERROR_DEFINITIONS
} nsmp_err_e;
#undef E

/* -------------------------------- Structs --------------------------------- */
/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Globals --------------------------------- */

// #define X(_, string) string,
// static const char* errorStrings[] = {ERROR_LIST};
// #undef X

/* -------------------------------- Locals ---------------------------------- */
/* -------------------------------- Functions ------------------------------- */
