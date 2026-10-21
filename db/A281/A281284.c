/*
 * A281284  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 350", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A281284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-4) - 16*a(n-8) for n > 23 initial terms: a(0)=1,
 *            a(1)=3, a(2)=3, a(3)=11, a(4)=27, a(5)=11, a(6)=59, a(7)=203,
 *            a(8)=123, a(9)=267, a(10)=1467, a(11)=4043, a(12)=123,
 *            a(13)=4363, a(14)=21947, a(15)=62667, a(16)=14203, a(17)=102667,
 *            a(18)=316859, a(19)=939211, a(20)=227195, a(21)=1642...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first 24 terms of the sequence, used as initial values */
    static const seq_t init[24] = {
        (seq_t)1,  (seq_t)3,  (seq_t)3,  (seq_t)11,
        (seq_t)27, (seq_t)11, (seq_t)59, (seq_t)203,
        (seq_t)123, (seq_t)267, (seq_t)1467, (seq_t)4043,
        (seq_t)123, (seq_t)4363, (seq_t)21947, (seq_t)62667,
        (seq_t)14203, (seq_t)102667, (seq_t)316859, (seq_t)939211,
        (seq_t)227195, (seq_t)1642763, (seq_t)5076411, (seq_t)15013067
    };

    /* For indices below 24 just return the known value */
    if (n < 24)
        return init[(idx_t)n];

    /* Static cache for computed values */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;   /* number of terms stored (>= 24 when initialized) */
    static size_t cache_cap = 0;   /* allocated array size */

    /* Lazy initialization of the cache */
    if (cache == NULL) {
        cache_cap = 64;                         /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* out of memory – caller may ignore */

        for (idx_t i = 0; i < 24; ++i)
            cache[i] = init[i];
        cache_len = 24;
    }

    /* Extend cache up to the requested index */
    if (n >= cache_len) {
        size_t needed = (size_t)(n + 1);
        if (needed > cache_cap) {
            /* grow capacity geometrically */
            size_t new_cap = cache_cap;
            while (needed > new_cap)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                return (seq_t)0;   /* out of memory – caller may ignore */
            cache   = tmp;
            cache_cap = new_cap;
        }

        for (idx_t i = cache_len; i <= n; ++i) {
            /* Recurrence: a(i) = 17*a(i-4) - 16*a(i-8) */
            cache[i] = (seq_t)17 * cache[i - 4] - (seq_t)16 * cache[i - 8];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
