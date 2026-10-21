/*
 * A269579  Number of length-n 0..4 arrays with no repeated value differing
 *          from the previous repeated value by more than one.
 * https://oeis.org/A269579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 93*a(n-2) + 220*a(n-3) - 112*a(n-4) - 192*a
 *            (n-5) - 2*a(n-6) + 8*a(n-7) Initial values: a(1)=5, a(2)=25,
 *            a(3)=125, a(4)=613, a(5)=2969, a(6)=14239, a(7)=67763
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t cached_len = 0;
    static idx_t  last_computed = 0;

    /* ensure we have space for values up to n */
    size_t needed = (size_t)n + 1;
    if (needed < 8)                 /* we always need at least a(1)…a(7) */
        needed = 8;
    if (cached_len < needed) {
        size_t new_len = cached_len ? cached_len : 8;
        while (new_len < needed)
            new_len <<= 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cached_len = new_len;
    }

    /* base values for n=1..7 */
    static const seq_t base[8] = {
        0,          /* index 0 unused to keep 1‑based indexing */
        5,          /* a(1) */
        25,         /* a(2) */
        125,        /* a(3) */
        613,        /* a(4) */
        2969,       /* a(5) */
        14239,      /* a(6) */
        67763       /* a(7) */
    };

    /* initialise cache for the first time */
    if (last_computed == 0 && cache[1] == (seq_t)0) {
        for (int i = 1; i <= 7; ++i)
            cache[i] = base[i];
        last_computed = 7;
    }

    /* compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = base[i];
        } else {
            seq_t term = cache[i - 1] * (seq_t)16
                        - cache[i - 2] * (seq_t)93
                        + cache[i - 3] * (seq_t)220
                        - cache[i - 4] * (seq_t)112
                        - cache[i - 5] * (seq_t)192
                        - cache[i - 6] * (seq_t)2
                        + cache[i - 7] * (seq_t)8;
            cache[i] = term;
        }
    }
    last_computed = n;
    return cache[n];
}
