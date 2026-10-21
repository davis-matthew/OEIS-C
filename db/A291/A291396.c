/*
 * A291396  p-INVERT of (1,1,0,0,0,0,...), where p(S) = (1 - S)(1 - 2 S)(1 - 3
 *          S).
 * https://oeis.org/A291396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A291396 – p-INVERT of (1,1,0,0,0,0,…), p(S) = (1‑S)(1‑2S)(1‑3S).
   Recurrence (for n ≥ 7):
   a(n) = 6·a(n‑1) – 5·a(n‑2) – 16·a(n‑3) + 7·a(n‑4) + 18·a(n‑5) + 6·a(n‑6)
   Initial values:
   a(0)=6, a(1)=31, a(2)=140, a(3)=596, a(4)=2440, a(5)=9751, a(6)=38344
*/

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;   /* allocated capacity */
    static size_t len     = 0;   /* number of terms currently stored */

    /* initialise cache on first call */
    if (!cache) {
        const size_t init_cap = 64;            /* arbitrary > 7 */
        cache = seq_alloc(init_cap);
        if (!cache) return (seq_t)0;            /* fallback on OOM */
        /* fill initial known values */
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)31;
        cache[2] = (seq_t)140;
        cache[3] = (seq_t)596;
        cache[4] = (seq_t)2440;
        cache[5] = (seq_t)9751;
        cache[6] = (seq_t)38344;
        len = 7;
        cap = init_cap;
    }

    /* return already computed term */
    if ((size_t)n < len)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n + 1)
            newcap = (size_t)n + 1;
        seq_t *tmp = seq_realloc(cache, cap, newcap);
        if (!tmp) return (seq_t)0; /* fallback on OOM */
        cache = tmp;
        cap   = newcap;
    }

    /* compute terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];

        /* compute using positive and negative parts separately
           to avoid unsigned underflow while computing in seq_t */
        seq_t pos = (seq_t)6 * a1
                  + (seq_t)7 * a4
                  + (seq_t)18 * a5
                  + (seq_t)6 * a6;
        seq_t neg = (seq_t)5 * a2
                  + (seq_t)16 * a3;

        cache[i] = pos - neg;
    }
    len = (size_t)n + 1;
    return cache[n];
}
