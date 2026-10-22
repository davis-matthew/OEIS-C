/*
 * A307181  a(n) is the first Zagreb index of the Lucas cube Lambda(n).
 * https://oeis.org/A307181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 5*a(n-3) + 3*a(n-5) + a(n-6) for n>8, a(1)=0,
 *            a(2)=6, a(3)=12, a(4)=40, a(5)=90, a(6)=216, a(7)=462, a(8)=976.
 *            This implementation uses a memoised recurrence with a dynamic
 *            cache
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i), 1‑based. */
    static seq_t *cache   = NULL;   /* array of terms */
    static idx_t cache_len = 0;     /* highest index that has been computed */

    /* initialise cache with the first eight terms (if not already done) */
    if (cache_len == 0) {
        /* allocate space for indices 0..8 (index 0 unused) */
        cache = (seq_t*)calloc(9, sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure → zero */

        cache[1] = (seq_t)0;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)40;
        cache[5] = (seq_t)90;
        cache[6] = (seq_t)216;
        cache[7] = (seq_t)462;
        cache[8] = (seq_t)976;
        cache_len = 8;
    }

    /* if the requested index is already cached, return it immediately */
    if (n <= cache_len)
        return cache[n];

    /* otherwise expand the cache up to n */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;          /* allocation failure → zero */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* recurrence is valid for i > 8 */
        if (i <= 8) {
            /* should not happen since we pre‑filled up to 8 */
            continue;
        }
        seq_t t1 = cache[i - 1] * (seq_t)3;        /* 3*a(i-1) */
        seq_t t2 = cache[i - 5] * (seq_t)3;        /* 3*a(i-5) */
        seq_t t3 = cache[i - 3] * (seq_t)5;        /* 5*a(i-3) */
        seq_t t4 = cache[i - 6];                   /* a(i-6)   */
        seq_t sum = t1 + t2 + t4;
        cache[i] = sum - t3;                       /* 3*a(i-1)+3*a(i-5)+a(i-6)-5*a(i-3) */
    }

    cache_len = n;
    return cache[n];
}
