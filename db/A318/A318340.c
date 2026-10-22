/*
 * A318340  Number of nX5 0..1 arrays with every element unequal to 0, 1, 2 or
 *          6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +5*a(n-2) -12*a(n-3) -17*a(n-4) +20*a(n-5)
 *            +31*a(n-6) -13*a(n-7) -33*a(n-8) +a(n-9) +20*a(n-10) +2*a(n-11)
 *            -4*a(n-12) -3*a(n-13) +a(n-15) for n > 18, with initial values
 *            a(1)..a(18) given below
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[18] = {
    16,                    /* a(1) */
    42,                    /* a(2) */
    76,                    /* a(3) */
    148,                   /* a(4) */
    393,                   /* a(5) */
    1115,                  /* a(6) */
    3321,                  /* a(7) */
    10111,                 /* a(8) */
    30943,                 /* a(9) */
    95244,                 /* a(10) */
    293401,                /* a(11) */
    904986,                /* a(12) */
    2792217,               /* a(13) */
    8617223,               /* a(14) */
    26597072,              /* a(15) */
    82096253,              /* a(16) */
    253412398,             /* a(17) */
    782236683              /* a(18) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, return 0 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms stored (a(1) at cache[0]) */

    /* Ensure initial 18 terms are in cache */
    if (cache_len < 18) {
        size_t new_cap = 18;
        if (cache == NULL)
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (idx_t i = cache_len; i < 18; ++i)
            cache[i] = init_vals[i];
        cache_len = 18;
    }

    /* If requested n is already computed, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend cache up to n */
    if (n > cache_len) {
        size_t new_size = n;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t idx = cache_len; idx < n; ++idx) {
            seq_t val;
            if (idx < 18) {
                /* idx is 0‑based; a(idx+1) = init_vals[idx] */
                val = init_vals[idx];
            } else {
                /* recurrence for k = idx+1 */
                val = 3 * cache[idx - 1]          /* a(n-1) */
                    + 5 * cache[idx - 2]          /* a(n-2) */
                    - 12 * cache[idx - 3]         /* a(n-3) */
                    - 17 * cache[idx - 4]         /* a(n-4) */
                    + 20 * cache[idx - 5]         /* a(n-5) */
                    + 31 * cache[idx - 6]         /* a(n-6) */
                    - 13 * cache[idx - 7]         /* a(n-7) */
                    - 33 * cache[idx - 8]         /* a(n-8) */
                    +   cache[idx - 9]            /* a(n-9) */
                    + 20 * cache[idx - 10]        /* a(n-10) */
                    + 2  * cache[idx - 11]        /* a(n-11) */
                    - 4  * cache[idx - 12]        /* a(n-12) */
                    - 3  * cache[idx - 13]        /* a(n-13) */
                    +   cache[idx - 15];          /* a(n-15) */
            }
            cache[idx] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
