/*
 * A376096  a(0) = 1; a(n) = Sum_{k=0..n-1} (k+1)^3 * a(k) * a(n-k-1).
 * https://oeis.org/A376096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} (k+1)^3 * a(k) * a(n-1-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for size_t, realloc, memset */

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* capacity of cache (number of slots) */

    /* enlarge cache if necessary */
    if ((idx_t)cap <= n) {
        size_t newcap = (size_t)n + 1;   /* include index n itself */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            return (seq_t)0;
        }
        /* initialize newly added elements to 0 */
        for (size_t i = cap; i < newcap; ++i) {
            tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cap   = newcap;
    }

    /* base case */
    if (cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
    }

    /* compute all missing values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) {        /* already known */
            continue;
        }
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t k1      = (seq_t)(k + 1);          /* (k+1)          */
            seq_t k1_cube = k1 * k1 * k1;            /* (k+1)^3        */
            seq_t term    = k1_cube;
            term = term * cache[k];
            term = term * cache[i - 1 - k];
            sum = sum + term;
        }
        cache[i] = sum;
    }

    return cache[n];
}
