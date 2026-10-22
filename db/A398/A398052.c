/*
 * A398052  Lower (1,2/5) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) Initial
 *            values: a(0)=0, a(1)=1, a(2)=4, a(3)=9, a(4)=16, a(5)=25,
 *            a(6)=35 (terms a(n) are defined for n ≥ 0)
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms stored in cache */

    /* initial 7 terms */
    static const seq_t init[7] = {
        (seq_t)0, (seq_t)1, (seq_t)4, (seq_t)9,
        (seq_t)16, (seq_t)25, (seq_t)35
    };

    /* initialise cache once */
    if (!cache) {
        cache_len = 7;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
    }

    /* if requested term already cached, return it */
    if (n < cache_len) return cache[n];

    /* need to compute up to n */
    idx_t old_len = cache_len;                /* old number of cached terms */
    idx_t new_len = n + 1;                    /* new required number of terms */

    /* extend cache if necessary */
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();
        cache = tmp;
        cache_len = new_len;
    }

    /* compute new terms from old_len to n */
    for (idx_t i = old_len; i <= n; ++i) {
        /* all indices i-1 .. i-7 are guaranteed to be < i, hence < cache_len */
        seq_t term = cache[i-1] * 2;         /* 2*a(i-1) */
        term -= cache[i-2];                 /* - a(i-2) */
        term += cache[i-5];                 /* + a(i-5) */
        term -= cache[i-6] * 2;             /* - 2*a(i-6) */
        term += cache[i-7];                 /* + a(i-7) */
        cache[i] = term;
    }

    return cache[n];
}
