/*
 * A291032  p-INVERT of the positive integers, where p(S) = 1 - S - S^2 - S^3
 *          + S^4.
 * https://oeis.org/A291032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 33*a(n-2) + 68*a(n-3) - 87*a(n-4) + 68*a(n-5)
 *            - 33*a(n-6) + 9*a(n-7) - a(n-8); Initial values: a(0)=1, a(1)=4,
 *            a(2)=15, a(3)=54, a(4)=188, a(5)=645, a(6)=2208, a(7)=7570
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /*  Static cache to store previously computed terms                   */
    /* ------------------------------------------------------------------ */
    static seq_t *cache   = NULL;   /* pointer to the array of seq_t          */
    static size_t cache_len = 0;    /* number of elements currently valid    */
    static size_t cache_cap = 0;    /* allocated capacity                    */

    /* ------------------------------------------------------------------ */
    /*  Initial values for n = 0..7                                         */
    /* ------------------------------------------------------------------ */
    static const seq_t init[8] = {1, 4, 15, 54, 188, 645, 2208, 7570};

    /* ------------------------------------------------------------------ */
    /*  Allocate and initialise cache on first call                        */
    /* ------------------------------------------------------------------ */
    if (!cache) {
        cache_cap = 8;
#if defined(__cplusplus)
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) abort(); /* allocation failure */
        /* copy initial values */
        for (size_t i = 0; i < 8; ++i) cache[i] = init[i];
        cache_len = 8;
    }

    /* ------------------------------------------------------------------ */
    /*  Return cached value if it already exists                           */
    /* ------------------------------------------------------------------ */
    if ((idx_t)cache_len > n) return cache[n];

    /* ------------------------------------------------------------------ */
    /*  Grow the array if necessary                                       */
    /* ------------------------------------------------------------------ */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1; /* double until big enough */
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        delete[] cache;
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /*  Compute missing terms using the 8‑term linear recurrence           */
    /* ------------------------------------------------------------------ */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t pos = ((seq_t)9  * cache[i-1]) +
                    ((seq_t)68 * cache[i-3]) +
                    ((seq_t)68 * cache[i-5]) +
                    ((seq_t)9  * cache[i-7]);

        seq_t neg = ((seq_t)33 * cache[i-2]) +
                    ((seq_t)87 * cache[i-4]) +
                    ((seq_t)33 * cache[i-6]) +
                    cache[i-8];

        cache[i] = pos - neg;
    }
    cache_len = (size_t)n + 1;

    /* ------------------------------------------------------------------ */
    /*  Return the requested term                                          */
    /* ------------------------------------------------------------------ */
    return cache[n];
}
