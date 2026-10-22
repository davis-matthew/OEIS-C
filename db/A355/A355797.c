/*
 * A355797  Row 4 of A355793.
 * https://oeis.org/A355797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 51, a(3) = 1119 For n ≥ 4: a(n) = 11
 *            · Σ_{k=0}^{n-1} a(k)·a(n-1-k) + (3n-11) · a(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoized values a(0)..a(last) */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t initialized = 0;    /* first uncomputed index */

    /* enlarge cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        new_cap = new_cap * 2;       /* over‑allocate to reduce reallocs */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* unlikely to happen */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute up to n, inclusive */
    for (idx_t i = initialized; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)3;
        } else if (i == 2) {
            cache[2] = (seq_t)51;
        } else if (i == 3) {
            cache[3] = (seq_t)1119;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < i; ++j) {
                sum += cache[j] * cache[i - 1 - j];
            }
            seq_t term = (seq_t)3 * (seq_t)i - (seq_t)11;   /* 3*n-11  (positive for n≥4) */
            cache[i] = (seq_t)11 * sum + term * cache[i - 1];
        }
    }

    initialized = n + 1;
    return cache[n];
}
