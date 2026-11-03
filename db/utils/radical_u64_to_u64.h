/* radical_u64_to_u64: the product of the distinct primes dividing n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_RADICAL_U64_TO_U64_H
#define OEISC_UTILS_RADICAL_U64_TO_U64_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Utility: radical of a 64‑bit number (product of distinct prime factors). */
static uint64_t radical_u64_to_u64(uint64_t x)
{
    uint64_t r = 1;
    if (x % 2 == 0) {
        r *= 2;
        while (x % 2 == 0) x /= 2;
    }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            r *= p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) r *= x;
    return r;
}

#endif
