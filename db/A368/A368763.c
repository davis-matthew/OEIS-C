/*
 * A368763  a(n) = n! * (1 + Sum_{k=0..n} binomial(k+2,3) / k!).
 * https://oeis.org/A368763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + binomial(n+2,3) with a(0)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated capacity */
    static size_t size = 0;  /* number of values stored */

    /* initialise cache on first call */
    if (size == 0) {
        cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return (seq_t)0;      /* unlikely, but defensive */
        cache[0] = (seq_t)1; /* a(0) */
        size = 1;
    }

    /* if result already cached, return it */
    if (n < size)
        return cache[(size_t)n];

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cap)
        grow_seq(&cache, &cap, (size_t)n + 1);

    /* compute missing values up to n */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        /* binomial(i+2,3) = (i+2)(i+1)i / 6 */
        seq_t t1 = (seq_t)(i + 2);
        seq_t t2 = (seq_t)(i + 1);
        seq_t t3 = (seq_t)i;
        seq_t binom = t1 * t2 * t3 / 6;
        cache[(size_t)i] = (seq_t)i * cache[(size_t)(i - 1)] + binom;
    }
    size = n + 1;
    return cache[(size_t)n];
}
