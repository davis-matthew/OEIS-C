/* mulmod_u32: a*b mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MULMOD_U32_H
#define OEISC_UTILS_MULMOD_U32_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --------- Miller–Rabin primality test for 32‑bit numbers --------- */
static uint32_t mulmod_u32(uint32_t a, uint32_t b, uint32_t mod)
{
    return (uint32_t)((unsigned __int128)a * b % mod);
}

#endif
