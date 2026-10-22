/*
 * A306848  Product of first n odd nonprimes, a(n) = Product_{k=1..n}
 *          A071904(k).
 * https://oeis.org/A306848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = product of the first n odd composite numbers (odd nonprimes), a(0)=1 */

/*  The function keeps a static cache of already computed values.
    For each new value it finds the next odd composite using the utility
    is_prime_small and multiplies it into the running product. */
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed values, indexed by n
       cache[n] holds a(n) and cache_len == max_n+1 */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* the next odd number that has yet to be examined; starts at 9 */
    static idx_t next_odd = 9;

    /* initialise cache for n==0 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;       /* a(0) = 1 */
        cache_len = 1;
    }

    /* extend cache up to n */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* start from the last computed product */
        seq_t product = cache[cache_len - 1];

        for (idx_t idx = cache_len; idx <= n; ++idx) {
            /* find the next odd composite */
            while (is_prime_small((seq_t)next_odd))
                next_odd += 2;    /* skip primes */

            seq_t composite = (seq_t)next_odd;
            product = product * composite;    /* multiply into running product */
            cache[idx] = product;

            next_odd += 2;           /* advance to next odd candidate */
        }

        cache_len = new_len;
    }

    return cache[n];
}
