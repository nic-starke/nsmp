/* -------------------------------------------------------------------------- */
/*                  Copyright (c) (2023 - 2024) Nicolaus Starke               */
/*                      https://github.com/nic-starke/nsmp                    */
/*                         SPDX-License-Identifier: MIT                       */
/* -------------------------------------------------------------------------- */
/* -------------------------------- Includes -------------------------------- */
#include <stddef.h>
#include "nsmp_uid.h"
/* -------------------------------- Defines / Externs ----------------------- */
/* -------------------------------- Types / Enums / Structs ----------------- */
/* -------------------------------- Declarations ---------------------------- */

static uint64_t xorshift128plus(uint64_t* s);

/* -------------------------------- Variables ------------------------------- */

static uint64_t s_seed[2] = {1, 2};

/* -------------------------------- Definitions ----------------------------- */

/**
 * @brief User must call this function before using any other uuid functions.
 *
 * @param seed The seed for the random number generator.
 * @return nsmp_err_e
 */
nsmp_err_e nsmp_uid_init(uint64_t seed) {
	if (seed == 0) {
		return NSMP_ERR_BAD_ARG;
	}

	s_seed[0] = seed;
	s_seed[1] = seed << 32 | seed >> 32;
	return NSMP_OK;
}

/**
 * @brief Generate a new unique identifier.
 * Definitily not secure, but good enough for random numbers.
 *
 * @param uid The buffer to store the new uid, even better
 * if you dont initialise it and just take whats on the stack.
 *
 * @return nsmp_err_e NSMP_OK on success.
 */
nsmp_err_e nsmp_uid_generate(char uid[NSMP_UID_LEN]) {
	if (uid == NULL) {
		return NSMP_ERR_NULL;
	}

	if (s_seed[0] == 1 || s_seed[1] == 2) {
		return NSMP_ERR_BAD_SEQ;
	}

	static const char* hex = "0123456789abcdef";

	/* get some stuff from the stack... */
	union {
		unsigned char b[16];
		uint64_t			word[2];
	} s;

	/* ... randomise! */
	s.word[0] = xorshift128plus(uid);
	s.word[1] = xorshift128plus(s_seed);

	/* generate the uid */
	for (size_t i = 0; i < NSMP_UID_LEN; i++) {
		int n	 = s.b[i >> 1];
		n			 = (i & 1) ? n >> 4 : n & 0xf;
		uid[i] = hex[n];
	}

	return NSMP_OK;
}

/**
 * @brief The xorshift128plus random number generator.
 *
 * @param s The seed.
 * @return uint64_t The next random number.
 */
static uint64_t xorshift128plus(uint64_t* s) {
	/* http://xorshift.di.unimi.it/xorshift128plus.c */
	uint64_t			 s1 = s[0];
	const uint64_t s0 = s[1];
	s[0]							= s0;
	s1 ^= s1 << 23;
	s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
	return s[1] + s0;
}
