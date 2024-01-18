/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */

#pragma once
#include "stdio.h"

#include "nsmp_cfg.h"

#define ERROR_DEFINITIONS                                                      \
                                                                               \
	E(NSMP_FAIL, "FAIL")                                                         \
	E(NSMP_OK, "OK")                                                             \
	E(NSMP_ERR_BAD_ARG, "Bad parameter/argument")                                \
	E(NSMP_ERR_BAD_CRC, "Bad CRC value")                                         \
	E(NSMP_ERR_BAD_LEN, "Bad message length")                                    \
	E(NSMP_ERR_BAD_VER, "Bad protocol version")                                  \
	E(NSMP_ERR_BAD_CFG, "Bad configuration")                                     \
	E(NSMP_ERR_BAD_ADDR, "Bad address")                                          \
	E(NSMP_ERR_BAD_TIME_FUNCTION, "Bad time function")                           \
	E(NSMP_ERR_NETIF_NOT_FOUND, "Interface not found")                           \
	E(NSMP_ERR_COBS_ENCODE, "COBS encoding error")                               \
	E(NSMP_ERR_NULL_PTR, "Null pointer")

#define E(code, str) code,
typedef enum {
	ERROR_DEFINITIONS
} nsmp_err_e;
#undef E

/* clang-format off */
#if NSMP_ENABLE_ERROR_PRINT

	/*
		Two different print functions are defined for tracing errors onto stderr.
		The first prints verbose information (if compiled with DEBUG). The second
		prints an error code - this can be then be interpreted later, effectively
		reducing the size of the release binaries (if compiled with NDEBUG). Its not
		quite defmt from rust but it will do for now.
	*/

	#ifdef DEBUG

		#define E(_, string) string,
		static const char* err_strings[] = {ERROR_DEFINITIONS};
		#undef E

		#define printerr(e)                                                         \
		if (e != NSMP_OK) 																													\
			fprintf(stderr, "%s:%d: %d - %s\n", __FILE__, __LINE__, e, err_strings[e])

	#endif /* DEBUG */

	#ifdef NDEBUG
		#define printerr(e) fprintf(stderr, "NSMP_ERR: %d\n", e)
	#endif

#else /* NSMP_ENABLE_ERROR_PRINT = 0 */
	#define printerr(e) ((void)0)
#endif

/* clang-format on */

#define RETURN_IF_ERR(b)                                                       \
	if ((b) != NSMP_OK) {                                                        \
		printerr(b);                                                               \
		return b;                                                                  \
	}

#define RETURN_IF_NULL(x)                                                      \
	if (NULL == (x))                                                             \
	return NSMP_ERR_NULL_PTR

#define RETURN_ERR_IF_FALSE(b, e)                                              \
	if (!(b)) {                                                                  \
		printerr(e);                                                               \
		return e;                                                                  \
	}
