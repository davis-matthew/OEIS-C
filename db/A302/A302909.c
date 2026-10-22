/*
 * A302909  Determinant of n X n matrix whose main diagonal consists of the
 *          first n 5-gonal numbers and all other elements are 1's.
 * https://oeis.org/A302909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑1) * n * (3*n + 5) / 2, a(1) = 1 (integer product)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, but safe */

    /* static cache for memoization: cache[i] holds a(i+1)  (0‑based) */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;

    /* enlarge cache if needed */
    if (n > cache_sz) {
        idx_t new_sz = n;
        if (cache == NULL) {
            cache = (seq_t*)malloc(new_sz * sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        }
        if (cache == NULL) return 0;   /* allocation failure – unlikely */

        /* initialise first value if cache was empty */
        if (cache_sz == 0) {
            cache[0] = (seq_t)1;
            cache_sz = 1;
        }

        /* compute missing terms up to a(n) */
        seq_t prev = cache[cache_sz - 1];   /* current a(cache_sz) */
        for (idx_t k = cache_sz; k < n; ++k) {
            /* factor = k * (3*k + 5) / 2  (k runs from cache_sz to n‑1) */
            seq_t tmp = (seq_t)k * 3 + 5;          /* 3*k + 5 */
            seq_t factor;
            if ((k & 1) == 0) {
                /* k even – divide k by 2 first */
                factor = ((seq_t)(k / 2)) * tmp;
            } else {
                /* (3*k+5) even – divide it by 2 */
                factor = (seq_t)k * (tmp / 2);
            }
            /* next value */
            seq_t val = prev * factor;
            cache[k] = val;
            prev = val;
        }
        cache_sz = n;
    }

    return cache[n - 1];
}
