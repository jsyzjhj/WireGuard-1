/* Copyright 2015-2016 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved. */

#ifndef CURVE25519_H
#define CURVE25519_H

#include <stdint.h>
#include <sys/types.h>

enum curve25519_lengths {
	CURVE25519_POINT_SIZE = 32,
};

void curve25519(uint8_t *mypublic, const uint8_t *secret, const uint8_t *basepoint);
void curve25519_generate_public(uint8_t *pub, const uint8_t *secret);
static inline void curve25519_normalize_secret(uint8_t secret[CURVE25519_POINT_SIZE])
{
	secret[0] &= 248;
	secret[31] &= 127;
	secret[31] |= 64;
}

#endif