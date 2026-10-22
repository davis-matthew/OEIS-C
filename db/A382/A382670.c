/*
 * A382670  Numbers k such that a regular k-gon (k>=3) can be constructed with
 *          a compass, straightedge and an angle quinsector.
 * https://oeis.org/A382670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   prime_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A382670: numbers k ≥ 3 such that every odd prime divisor p of k satisfies
   p−1 has no prime factor other than 2 or 5, and for p≠5 the exponent of
   p in k is 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/prime_ok.h"

/* Test whether k satisfies the sequence condition. */
static bool ok_k(uint64_t k)
{
    if (k < 3)
        return false;

    /* Remove the factor 2^e (always allowed). */
    uint64_t odd = k;
    while ((odd & 1ULL) == 0)
        odd >>= 1;

    if (odd == 1)
        return true;        /* k is a power of 2 */

    uint64_t temp = odd;
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            if (!prime_ok(p))
                return false;
            /* Count exponent of p. */
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (p != 5 && exp > 1)
                return false;   /* exponent of non‑5 prime > 1 is forbidden */
        }
    }

    if (temp > 1) {          /* remaining prime factor */
        if (!prime_ok(temp))
            return false;
        /* exp = 1 automatically; if temp != 5 it's acceptable */
    }

    return true;
}

/* Static cache of generated sequence values. */
static seq_t *seq_vals   = NULL;   /* expanded storage */
static size_t seq_len    = 0;
static size_t seq_cap    = 0;
static uint64_t last_k   = 2;      /* last k examined */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for offset 0 */

    while (seq_len < (size_t)n) {
        uint64_t k = ++last_k;    /* next integer to test */

        if (!ok_k(k))
            continue;               /* skip invalid k */

        /* Ensure capacity. */
        if (seq_len == seq_cap) {
            size_t new_cap = seq_cap ? seq_cap << 1 : 8;
            seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
            if (!tmp)                /* allocation failed */
                return 0;
            seq_vals = tmp;
            seq_cap  = new_cap;
        }

        seq_vals[seq_len++] = (seq_t)k;
    }

    return seq_vals[n - 1];
}
