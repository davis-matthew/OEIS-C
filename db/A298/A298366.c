/*
 * A298366  Even numbers n such that n-5 and n-3 are both composite.
 * https://oeis.org/A298366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 7000, u32 n <= 7000, u64 n <= 7000,
 *            u128 n <= 7000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298366: a(n) is the nth even integer ≥ 30 for which both
   n-5 and n-3 are composite.  The function searches even numbers
   sequentially, storing the found terms in a cache so that any call
   order is handled efficiently. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0)                  /* a(0) is not defined; safe return */
        return (seq_t)0;

    /* Static cache of computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next even number to test; start at the first candidate 30 */
    static uint64_t next_candidate = 30ULL;

    /* Grow the cache until it contains at least n terms */
    while (cache_len < n) {
        seq_t cand = (seq_t)next_candidate;      /* cast to seq_t */

        /* Test the primality of cand-5 and cand-3 using the supplied helper */
        seq_t val1 = cand - (seq_t)5;
        seq_t val2 = cand - (seq_t)3;
        bool comp1 = !is_prime_small(val1);
        bool comp2 = !is_prime_small(val2);

        if (comp1 && comp2) {                      /* found a valid term */
            /* Expand cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failed – abort with zero */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        next_candidate += 2ULL;                      /* next even number */
    }

    return cache[n - 1];
}
