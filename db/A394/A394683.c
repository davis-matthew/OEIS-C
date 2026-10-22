/*
 * A394683  a(n) = (m*p)^2 + 1 where p = A002496(n) is the n-th prime of the
 *          form m^2 + 1.
 * https://oeis.org/A394683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (m*(m^2+1))^2 + 1, where m^2+1 is the n‑th prime of the
 *            form m^2+1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored   */
    static size_t cache_cap  = 0;   /* allocated capacity       */
    static idx_t  last_m     = 0;   /* last m that produced a prime */

    /* Grow the cache until it contains the requested term. */
    while (cache_len < (size_t)n) {
        /* Find the next m such that m^2 + 1 is prime. */
        idx_t m = last_m + 1;
        seq_t p;
        for (;;) {
            p = (seq_t)m * (seq_t)m + 1;
            if (is_prime_big(p))
                break;
            ++m;
        }
        last_m = m;

        /* Compute a(n) = (m*p)^2 + 1. */
        seq_t mp   = (seq_t)m * p;       /* m * p                              */
        seq_t term = mp * mp + 1;        /* (m*p)^2 + 1                        */

        /* Ensure the cache array has enough capacity. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) { exit(EXIT_FAILURE); }
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = term;
    }

    return cache[n - 1];
}
