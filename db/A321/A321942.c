/*
 * A321942  A sequence related to the Euler-Gompertz constant.
 * https://oeis.org/A321942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 2, a(n) = (2*n-1)*a(n-1) - (n-1)*(n-2)*a(n-2)
 *            for n > 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for computed values (1-indexed) */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* array capacity (number of entries) */
    static idx_t max_calc = 0;      /* highest index computed so far */

    /* if cache is not large enough, enlarge it */
    if (n >= cap) {
        size_t new_cap = (size_t)n + 1;   /* we need indices 0..n */
        if (!cache) {
            cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!cache) {
            /* on a serious allocation failure fall back to a dummy zero */
            return (seq_t)0;
        }
        cap = new_cap;
    }

    /* initialise base values if not already present */
    if (cap > 1) cache[1] = (seq_t)1;
    if (cap > 2) cache[2] = (seq_t)2;

    /* compute new values if needed */
    if (max_calc < n) {
        idx_t start = max_calc + 1;
        if (start < 3) start = 3;           /* start from first non‑base index */

        for (idx_t m = start; m <= n; ++m) {
            seq_t term1 = cache[m-1];
            seq_t term2 = cache[m-2];
            seq_t coef1 = (seq_t)(2*m - 1);
            seq_t coef2 = (seq_t)((m-1)*(m-2));
            cache[m] = coef1 * term1 - coef2 * term2;
        }
        max_calc = n;
    }

    return cache[n];
}
