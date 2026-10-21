/*
 * A290913  p-INVERT of the positive integers, where p(S) = 1 - 7*S^2.
 * https://oeis.org/A290913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=7, a(2)=28, a(3)=119,
   a(n)=4*a(n-1)+a(n-2)+4*a(n-3)-a(n-4) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated capacity */
    static size_t len = 0;      /* next index to compute */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;
    }

    /* grow cache if necessary */
    size_t m = (size_t)n;
    if (m >= cap) {
        size_t newcap = cap;
        while (m >= newcap) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
        cap = newcap;
    }

    /* initialise first four terms once */
    if (len < 4) {
        cache[0] = 0;
        cache[1] = 7;
        cache[2] = 28;
        cache[3] = 119;
        len = 4;
    }

    /* compute missing terms up to n */
    for (size_t i = len; i <= m; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        cache[i] = 4 * t1 + t2 + 4 * t3 - t4;
    }
    len = m + 1;

    return cache[m];
}
