/*
 * A259483  Numbers n such that both n*log(2) and n*log(3) are within
 *          1/sqrt(n) of integers.
 * https://oeis.org/A259483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259483: numbers n such that both n⋅log(2) and n⋅log(3)
   lie within 1/√n of an integer.  The function builds the list
   incrementally; subsequent calls reuse the previously computed
   terms.  All arithmetic on seq_t is integer; floating‑point
   calculations are performed only inside the test helper. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Static cache for the sequence values */
static seq_t *cache = NULL;         /* array of seq_t values */
static idx_t  cache_used   = 0;     /* number of values stored */
static idx_t  cache_cap    = 0;     /* current capacity of the array */
static uint64_t next_candidate = 1; /* next integer to test */

/* Test whether an integer m satisfies the A259483 condition */
static bool is_candidate(uint64_t m)
{
    /* Check n*log(2) */
    double d2 = m * log(2.0);
    double r2 = round(d2);
    if (fabs(d2 - r2) >= 1.0 / sqrt((double)m))
        return false;

    /* Check n*log(3) */
    double d3 = m * log(3.0);
    double r3 = round(d3);
    if (fabs(d3 - r3) >= 1.0 / sqrt((double)m))
        return false;

    return true;
}

/* Grow the cache until it contains at least n terms */
static void ensure_cache(idx_t n)
{
    if (cache_used >= n)
        return;

    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    while (cache_used < n) {
        /* Find the next candidate */
        while (!is_candidate(next_candidate))
            ++next_candidate;

        /* Store the candidate */
        if (cache_used == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_used++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* Public interface required by the benchmark harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined; safety value */

    ensure_cache(n);
    return cache[n - 1];
}
