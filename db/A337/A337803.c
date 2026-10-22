/*
 * A337803  Odd integers k not divisible by 5, such that dr(k) divides k-1 or
 *          k+1, where dr(k) is the additive digital root of k (A010888).
 * https://oeis.org/A337803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337803: odd integers k (k%5≠0) such that dr(k) divides k-1 or k+1,
   where dr(k) is the additive digital root of k (dr(k) = 9 if k%9==0, otherwise k%9).
   Algorithm: iterate over odd k, test the divisibility condition, and collect
   values until the requested index n is reached. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stored sequence elements            */
static size_t cache_size = 0;        /* number of elements currently stored */
static size_t cache_cap   = 0;       /* current allocation capacity         */
static seq_t next_k = 1;             /* next odd integer to test            */

seq_t a(idx_t n) {
    /* Expand the cache until we have at least n elements. */
    while (cache_size < (size_t)n) {
        seq_t k = next_k;            /* current odd integer                */
        next_k += 2;                 /* prepare next odd integer            */

        /* skip numbers divisible by 5 */
        if ((k % 5) == 0)
            continue;

        /* compute additive digital root: 9 if k%9==0 else k%9 */
        seq_t dr = k % 9;
        if (dr == 0)
            dr = 9;

        /* check divisibility of k-1 or k+1 by dr */
        if (((k - 1) % dr == 0) || ((k + 1) % dr == 0)) {
            /* enlarge the cache if needed */
            if (cache_size == cache_cap) {
                cache_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = k;
        }
    }

    return cache[(size_t)n - 1];
}
