/* mulmod: a*b mod m.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_MULMOD_H
#define OEISC_UTILS_MULMOD_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---- Miller–Rabin helpers for 64‑bit integers ---- */
static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (unsigned __int128)a * b % m;
}

#endif
