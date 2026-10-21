/*
 * A269582  Number of length-n 0..7 arrays with no repeated value differing
 *          from the previous repeated value by more than one.
 * https://oeis.org/A269582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*a(n-1) -485*a(n-2) +3290*a(n-3) -10541*a(n-4)
 *            +10073*a(n-5) * +11122*a(n-6) +2072*a(n-7) -47*a(n-8) -14*a(n-9)
 *            * Initial values (n = 1..9): * 8, 64, 512, 4054, 31844, 248464,
 *            1927694, 14883506, 114432704
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation *//* 1-indexed storage */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initial values */
    static const seq_t init_vals[9] = {
        (seq_t)8,       /* a(1) */
        (seq_t)64,      /* a(2) */
        (seq_t)512,     /* a(3) */
        (seq_t)4054,    /* a(4) */
        (seq_t)31844,   /* a(5) */
        (seq_t)248464,  /* a(6) */
        (seq_t)1927694, /* a(7) */
        (seq_t)14883506,/* a(8) */
        (seq_t)114432704/* a(9) */
    };

    if (n == 0) return (seq_t)0; /* value undefined for n < 1 */

    /* ensure cache at least length 9 (initial values) */
    if (cache_len < 9) {
        size_t new_len = 9;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (size_t i = cache_len; i < 9; ++i)
            cache[i] = init_vals[i];
        cache_len = 9;
    }

    /* if requested value already cached, return it */
    if ((idx_t)cache_len >= n)
        return cache[(size_t)n - 1];

    /* coefficients for the recurrence (index 0 -> a(n-1) term) */
    static const int coef[9] = {35, -485, 3290, -10541, 10073, 11122, 2072, -47, -14};

    /* extend cache to required size n */
    size_t need = (size_t)n;
    cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    /* compute terms from cache_len+1 up to n */
    for (size_t m = cache_len + 1; m <= need; ++m) {
        /* compute a(m) using recurrence */
        seq_t posSum = 0;
        seq_t negSum = 0;
        for (int k = 0; k < 9; ++k) {
            seq_t term = cache[m - 2 - (size_t)k]; /* m-1-k, 0‑based index */
            int c = coef[k];
            if (c > 0)
                posSum += (seq_t)c * term;
            else
                negSum += (seq_t)(-c) * term;
        }
        cache[m - 1] = posSum - negSum; /* result fits in seq_t for this build */
    }
    cache_len = need;
    return cache[(size_t)n - 1];
}
