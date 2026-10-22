/*
 * A351650  Integers m such that digsum(m) divides digsum(m^2) where digsum =
 *          sum of digits = A007953.
 * https://oeis.org/A351650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer m such that digsum(m) | digsum(m^2),
 *            where digsum denotes the sum of decimal digits (OEIS A007953)
 * Range:     u8 n <= 8, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;        /* cached terms of the sequence */
static size_t cache_cnt = 0;       /* number of cached terms */
static size_t cache_cap = 0;       /* current capacity of the cache */
static idx_t next_m = 1;           /* next integer to test for the property */

/* Ensure the cache contains at least 'target' terms. */
static void ensure_cache(idx_t target)
{
    if (cache_cnt >= target) return;

    /* Expand cache capacity if necessary. */
    if (cache_cap == 0) cache_cap = 16;
    while (cache_cap < (size_t)target) cache_cap *= 2;

    if (cache == NULL)
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));

    /* Generate terms until we reach 'target' terms. */
    while (cache_cnt < target) {
        seq_t m = (seq_t)next_m;
        seq_t digm = digit_sum(m);
        seq_t sm   = m * m;               /* m^2 (fits in seq_t for required range) */
        seq_t digsm = digit_sum(sm);

        if (digm != 0 && digsm % digm == 0)
            cache[cache_cnt++] = m;

        ++next_m;
    }
}

/* Return the n-th term of A351650 (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined behavior for n < 1; return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
