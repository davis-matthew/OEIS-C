/*
 * A306611  The middle coefficient in the minimal polynomial for
 *          (2*cos(Pi/15))^n.
 * https://oeis.org/A306611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) + 5*a(n-2) + 25*a(n-3) + 5*a(n-4) - 4*a(n-5) -
 *            a(n-6) a(1) = -4, a(2) = 26, a(3) = -49, a(4) = 246, a(5) =
 *            -619, a(6) = 2621 Offset: 1
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 16, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types are provided by the build system */
typedef idx_t idx_t;   /* already defined elsewhere */

/* Memoised computation of the sequence */
seq_t a(idx_t n)
{
    /* 1‑based cache: cache[0] remains unused to keep indices readable */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;  /* highest index currently stored */
    static idx_t cache_cap   = 0;  /* allocated capacity (>= cache_size) */

    if (n == 0) /* undefined in the sequence – return 0 safely */
        return (seq_t)0;

    /* Grow the cache if we need a larger index */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 8;
        while (new_cap < n)
            new_cap <<= 1;                        /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1)        cache[i] = (seq_t)-4;
        else if (i == 2)   cache[i] = (seq_t)26;
        else if (i == 3)   cache[i] = (seq_t)-49;
        else if (i == 4)   cache[i] = (seq_t)246;
        else if (i == 5)   cache[i] = (seq_t)-619;
        else if (i == 6)   cache[i] = (seq_t)2621;
        else
            cache[i] = -( (seq_t)4 * cache[i-1] )
                       + ( (seq_t)5 * cache[i-2] )
                       + ( (seq_t)25 * cache[i-3] )
                       + ( (seq_t)5 * cache[i-4] )
                       - ( (seq_t)4 * cache[i-5] )
                       - cache[i-6];
    }

    cache_size = n;
    return cache[n];
}
