/*
 * A261471  Cyclotomic polynomial value Phi(6,n!).
 * https://oeis.org/A261471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 - n! + 1 (A002061(n!) = n!^2 - n! + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 97 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* Cache factorial values up to the maximum n seen.  */
    static seq_t *fact_cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize cache with capacity for n up to 10000.  */
    if (fact_cache == NULL) {
        /* 10001 entries: 0 … 10000 inclusive.  */
        fact_cache = mem_new(seq_t, (size_t)10001);
        fact_cache[0] = (seq_t)1;
        cache_len   = 1;
    }

    /* Extend the cache if a larger n is requested.  */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        cache_len = n + 1;
    }

    seq_t f = fact_cache[n];
    return f * f - f + (seq_t)1;
}
