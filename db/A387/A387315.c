/*
 * A387315  Expansion of 1/((1-x) * (1-13*x))^(5/2).
 * https://oeis.org/A387315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   n*a(n) = (14*n+21)*a(n-1) - 13*(n+3)*a(n-2), a(0)=1, a(1)=35
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A387315.  The result is
 * memoised in a static cache so that calls in any order are fast.
 */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* table of computed terms        */
    static size_t cache_cap = 0;       /* allocated size of the table    */
    static size_t cache_len = 0;       /* number of terms already stored */

    /* Grow the cache to accommodate the requested index. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed – unlikely */
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;

        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)35;
        } else {
            /* Use 128‑bit arithmetic for small built‑ins. */
            if (sizeof(seq_t) <= 8) {
                unsigned __int128 term1 = (unsigned __int128)(14 * i + 21)
                                         * (unsigned __int128)cache[i - 1];
                unsigned __int128 term2 = (unsigned __int128)13 * (i + 3)
                                         * (unsigned __int128)cache[i - 2];
                unsigned __int128 numer = term1 - term2;
                val = (seq_t)(numer / (unsigned __int128)i);
            } else {
                /* Use ordinary seq_t operations for arbitrary‑precision. */
                seq_t coef1 = (seq_t)(14 * i + 21);
                seq_t coef2 = (seq_t)(13 * (i + 3));
                seq_t term1 = coef1 * cache[i - 1];
                seq_t term2 = coef2 * cache[i - 2];
                seq_t numer = term1 - term2;
                val = numer / (seq_t)i;
            }
        }

        cache[i] = val;
        if (cache_len <= (size_t)i) cache_len = (size_t)i + 1;
    }

    return cache[n];
}
