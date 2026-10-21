/*
 * A284053  Relative of Hofstadter Q-sequence.
 * https://oeis.org/A284053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-a(n-1)) + a(n-a(n-2)) Initial values: a(1)=9,
 *            a(2)=20, a(3)=5, a(4)=5, a(5)=20, a(6)=9, a(7)=20, a(8)=5,
 *            a(9)=5, a(10)=20, a(11)=9, a(12)=5, a(13)=10, a(14)=10,
 *            a(15)=20; a(n)=0 for n<=0
 * Range:     u8 n <= 164, u16 n <= 9079, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/ensure_cache.h"

/* sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state ------------------------------------------------------ */
    static seq_t *cache   = NULL;  /* array of values, 1‑based */
    static size_t   cap    = 0;    /* allocated size (>= n+1)          */
    static idx_t    last   = 0;    /* highest computed index            */

    /* lazy initialization ---------------------------------------------- */
    if (!cache) {
        /* at least indices 0..15 */
        cap   = 16;                      /* indices 0‑15, 16 elements       */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failed */
        for (size_t i = 0; i < cap; ++i) cache[i] = 0;   /* a(n)=0 for n<=0 */

        /* set the first 15 terms */
        cache[1] = 9;  cache[2] = 20;  cache[3] = 5;
        cache[4] = 5;  cache[5] = 20;  cache[6] = 9;
        cache[7] = 20; cache[8] = 5;   cache[9] = 5;
        cache[10] = 20; cache[11] = 9; cache[12] = 5;
        cache[13] = 10; cache[14] = 10; cache[15] = 20;
        last = 15;
    }

    /* if already computed, return */
    if (n <= last)
        return cache[n];

    /* grow the cache so that it can hold indices up to n */
    if (n >= cap)
        ensure_cache(n, &cache, &cap);

    /* compute new terms up to n -------------------------------------- */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];

        /* calculate indices with safe comparison */
        idx_t idx1 = 0;
        if ((seq_t)prev1 <= (seq_t)i)       /* a(i‑1) <= i  */
            idx1 = i - (idx_t)prev1;        /* positive offset */

        idx_t idx2 = 0;
        if ((seq_t)prev2 <= (seq_t)i)       /* a(i‑2) <= i  */
            idx2 = i - (idx_t)prev2;

        seq_t term1 = (idx1 == 0) ? 0 : cache[idx1];
        seq_t term2 = (idx2 == 0) ? 0 : cache[idx2];
        cache[i]   = term1 + term2;        /* recurrence */
    }

    last = n;
    return cache[n];
}
