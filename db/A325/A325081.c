/*
 * A325081  Prime numbers congruent to 4, 9, 14, 34 or 49 modulo 55
 *          representable by x^2 + x*y + 14*y^2.
 * https://oeis.org/A325081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A325081:
   Prime numbers congruent to 4, 9, 14, 34 or 49 modulo 55 that are
   representable by the quadratic form x^2 + x*y + 14*y^2.
   The function returns the n‑th term (offset 1) by iterating over
   integers, testing primality with is_prime_big, and checking the
   representability via the discriminant test:
     Δ = 4*p – 55*y^2
   Δ must be a perfect square and Δ – y must be even.  Results are cached
   so that random access to n works efficiently. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility functions provided by the build environment */
#include "../utils/is_prime_big.h"
#include "../utils/floor_sqrt.h"

/* Cache of found terms */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static idx_t  cache_len  = 0;   /* number of terms stored in the cache */
static seq_t  last_checked = 1; /* last integer examined */

/* Check whether a prime p can be expressed as x^2 + x*y + 14*y^2. */
static bool representable(seq_t p)
{
    seq_t fourp = p * 4;

    for (seq_t y = 0; ; ++y) {
        /* y^2 */
        seq_t ysq = y * y;

        seq_t sub = 55 * ysq;
        if (sub > fourp) break;          /* 4p – 55*y^2 < 0 */

        seq_t tval = fourp - sub;        /* Δ = 4p – 55*y^2 */
        seq_t t = floor_sqrt(tval);      /* floor sqrt of Δ */

        if (t * t != tval) continue;     /* Δ is not a perfect square */

        /* (Δ – y) must be even for integer x to exist */
        if (((t - y) & 1) == 0) return true;
    }

    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* the sequence is 1‑based; index 0 unused */

    /* Initialise cache if needed */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        ++last_checked;   /* next integer to examine */

        /* Skip numbers that cannot be congruent to any of the allowed residues */
        int r = (int)(last_checked % 55);
        if (r != 4 && r != 9 && r != 14 && r != 34 && r != 49)
            continue;

        /* Must be prime */
        if (!is_prime_big(last_checked))
            continue;

        /* Must be representable by the given quadratic form */
        if (representable(last_checked)) {
            /* Expand cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = last_checked;
        }
    }

    /* Return the requested term */
    return cache[n - 1];
}
