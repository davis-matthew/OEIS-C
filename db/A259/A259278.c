/*
 * A259278  Number of compositions of n into parts 1, 6, and 7.
 * https://oeis.org/A259278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-6) + a(n-7); initial values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=1
 * Range:     u8 n <= 20, u16 n <= 38, u32 n <= 74, u64 n <= 145,
 *            u128 n <= 287, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;     /* memoised values */
    static size_t cap     = 0;       /* allocated size */
    static idx_t max_cal  = 0;       /* largest n computed so far  */

    /* initialise for the first call: indices 0..5 */
    if (cache == NULL) {
        /* store values 0..5 (six entries) */
        cap = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) { return 0; }   /* fatal failure; caller must handle */
        for (idx_t i = 0; i <= 5; ++i) cache[i] = (seq_t)1;
        max_cal = 5;
    }

    /* If requested n already computed, return it */
    if (n <= max_cal) return cache[n];

    /* Extend the cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) { return 0; } /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = max_cal + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        if (i >= 6) val += cache[i - 6];
        if (i >= 7) val += cache[i - 7];
        cache[i] = val;
    }
    max_cal = n;
    return cache[n];
}
