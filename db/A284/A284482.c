/*
 * A284482  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 950", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A284482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence:  a(n) = 2*a(n-1) + a(n-4) - 2*a(n-5)  for n ≥ 9
   Initial values:
     a(0) = 1, a(1) = 3, a(2) = 5, a(3) = 15, a(4) = 29,
     a(5) = 63, a(6) = 125, a(7) = 247, a(8) = 509   */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms stored in cache */

    /* initialise cache with base cases if needed */
    if (cache_size == 0) {
        cache = (seq_t *)realloc(NULL, (size_t)9 * sizeof(seq_t));
        if (!cache) { abort(); }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)15;
        cache[4] = (seq_t)29;
        cache[5] = (seq_t)63;
        cache[6] = (seq_t)125;
        cache[7] = (seq_t)247;
        cache[8] = (seq_t)509;
        cache_size = 9;
    }

    /* if requested term is already computed, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* grow cache to accommodate up to n */
    seq_t *new_cache = (seq_t *)realloc(cache,
                                        (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache) {
        abort();
    }
    cache = new_cache;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t = cache[i - 1];
        t <<= 1;                     /* t = 2*a(i-1) */
        t += cache[i - 4];            /* t += a(i-4) */
        seq_t t2 = cache[i - 5];
        t2 <<= 1;                     /* t2 = 2*a(i-5) */
        t -= t2;                      /* t -= 2*a(i-5) */
        cache[i] = t;
    }

    cache_size = n + 1;
    return cache[n];
}
