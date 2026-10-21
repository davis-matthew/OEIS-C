/*
 * A244764  Prime numbers ending in the prime number 17.
 * https://oeis.org/A244764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 169, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n‑th prime congruent to 17 modulo 100   */
/* (= prime of the form 100*k + 17).  The function keeps a static cache
 * of found terms and expands it on demand.  It uses the helper
 * is_prime_small(seq_t) to test primality. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static seq_t *cache     = NULL;   /* cache of 100*k+17 primes      */
static size_t cache_len = 0;       /* number of cached terms        */
static idx_t  next_k    = 0;       /* next candidate index to test  */

seq_t a(idx_t n)
{
    /* OEIS offset is 1.  Behaviour for n < 1 is undefined. */
    if (n == 0) return 0;

    /* Allocate or grow the cache to hold at least n terms */
    if (n > cache_len)
    {
        if (n > cache_len)
        {
            seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failure   */
            cache = tmp;
        }

        /* Find further primes of the form 100*k + 17 */
        for (; cache_len < n; )
        {
            seq_t cand = (seq_t)100 * next_k + 17;
            if (is_prime_small(cand))
            {
                cache[cache_len++] = cand;
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
