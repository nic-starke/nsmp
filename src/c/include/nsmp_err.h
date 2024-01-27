/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
#pragma once
/* -------------------------------- Includes -------------------------------- */
/* -------------------------------- Defines / Externs ----------------------- */

#define ERROR_DEFINITIONS                                                      \
	E(NSMP_OK, "OK")                                                             \
	E(NSMP_FAIL, "FAIL")                                                         \
	E(NSMP_ERR_BAD_ARG, "Bad parameter/argument")                                \
	E(NSMP_ERR_BAD_CRC, "Bad CRC value")                                         \
	E(NSMP_ERR_BAD_LEN, "Bad message length")                                    \
	E(NSMP_ERR_BAD_VER, "Bad protocol version")                                  \
	E(NSMP_ERR_BAD_CFG, "Bad configuration")                                     \
	E(NSMP_ERR_BAD_ADDR, "Bad address")                                          \
	E(NSMP_ERR_BAD_FILEDESC, "Bad file descriptor")                              \
	E(NSMP_ERR_BAD_BAUD, "Bad baudrate")                                         \
	E(NSMP_ERR_BAD_SEQ, "Bad operating sequence")                                \
	E(NSMP_ERR_BAD_TIME_FUNCTION, "Bad time function")                           \
	E(NSMP_ERR_NETIF_NOT_FOUND, "Interface not found")                           \
	E(NSMP_ERR_COBS_ENCODE, "COBS encoding error")                               \
	E(NSMP_ERR_EXTERNAL, "External error")                                       \
	E(NSMP_ERR_TRANSMIT, "Transmit error")                                       \
	E(NSMP_ERR_RECEIVE, "Receive error")                                         \
	E(NSMP_ERR_NULL, "Null pointer")

/* -------------------------------- Enums / Structs ------------------------- */

#define E(code, str) code,
typedef enum {
	ERROR_DEFINITIONS NSMP_ERR_NB
} nsmp_err_e;
#undef E

/* -------------------------------- Declarations ---------------------------- */
/* -------------------------------- Variables ------------------------------- */
/* -------------------------------- Definitions ----------------------------- */
