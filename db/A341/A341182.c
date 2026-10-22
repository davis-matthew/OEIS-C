/*
 * A341182  Numbers that when divided by the sum of their digits leave 15 as
 *          remainder.
 * https://oeis.org/A341182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2973, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   digit_sum, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A341182: the n‑th integer m such that m % digit_sum(m) == 15 */
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;          /* cached terms */
static idx_t cache_len = 0;          /* number of cached terms */
static idx_t cache_cap = 0;          /* current capacity of cache */
static seq_t next_candidate = 1;     /* next integer to test */

seq_t a(idx_t n)
{
    /* Build the cache up to the requested index. */
    while (cache_len < n)
    {
        seq_t cand = next_candidate++;          /* current candidate integer */
        seq_t s = digit_sum(cand);              /* sum of its decimal digits */

        if (cand % s == 15)                     /* condition satisfied */
        {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap)
            {
                if (cache == NULL)
                {
                    cache_cap = 16;
                    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
                }
                else
                {
                    ensure_capacity((size_t)(cache_len + 1),
                                    &cache_cap,
                                    (void **)&cache,
                                    sizeof(seq_t));
                }
            }
            cache[cache_len++] = cand;           /* store the term */
        }
    }
    return cache[n - 1];   /* 1‑based indexing */
}
