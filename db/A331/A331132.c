/*
 * A331132  a(n) = Sum_{i=1..n} d(i)^2*d(i+1), where d(n) = A000005(n).
 * https://oeis.org/A331132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} d(i)^2 * d(i+1) (offset 1)
 * Range:     u8 n <= 11, u16 n <= 323, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Static cache for divisor counts and cumulative sums */
static size_t cache_limit = 0;          /* highest n for which cache is valid   */
static uint32_t *divcnt = NULL;        /* divisor counts up to cache_limit+1   */
static seq_t   *cumsum  = NULL;        /* partial sums a(i)  (i from 0)       */

/* Helper: rebuild cache up to given limit */
static void rebuild_cache(idx_t limit)
{
    /* Allocate larger arrays if needed */
    if (cache_limit < (size_t)limit)
    {
        size_t new_size = (size_t)limit + 2;          /* we need i+1 up to limit+1 */
        uint32_t *new_divcnt = (uint32_t *)realloc(divcnt, new_size * sizeof(uint32_t));
        seq_t   *new_cumsum  = (seq_t *)  realloc(cumsum , new_size * sizeof(seq_t ));
        if (!new_divcnt || !new_cumsum)
        {
            /* In a robust program we would handle OOM; here we simply abort */
            exit(EXIT_FAILURE);
        }
        /* Initialize new entries to 0 */
        memset(new_divcnt + cache_limit + 1, 0, (new_size - cache_limit - 1) * sizeof(uint32_t));
        divcnt = new_divcnt;
        cumsum = new_cumsum;
        cache_limit = new_size - 2;   /* new upper bound for n */
    }

    /* Recompute divisor counts for all i up to limit+1 */
    size_t n = (size_t)limit + 1;
    for (size_t i = 1; i <= n; ++i)
        divcnt[i] = 0;  /* reset */
    for (size_t i = 1; i <= n; ++i)
    {
        for (size_t j = i; j <= n; j += i)
            divcnt[j] += 1;
    }

    /* Compute cumulative sums a(i) */
    cumsum[0] = 0;
    seq_t s = 0;
    for (size_t i = 1; i <= limit; ++i)
    {
        seq_t k = (seq_t)divcnt[i];
        seq_t term = k * k * (seq_t)divcnt[i + 1];
        s += term;
        cumsum[i] = s;
    }
}

/* Main function requested by the problem */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* out of bounds, but not used */
    if (cache_limit < (size_t)n)
        rebuild_cache(n);

    return cumsum[(size_t)n];
}
