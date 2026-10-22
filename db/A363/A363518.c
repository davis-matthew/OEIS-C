/*
 * A363518  Concentric square numbers on the faces of an n X n X n cube.
 * https://oeis.org/A363518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 4*a(n-3) - 3*a(n-4) + a(n-5) for n
 *            > 6, with initial values: a(1)=1, a(2)=8, a(3)=20, a(4)=32,
 *            a(5)=50, a(6)=80
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* number of terms stored (max index) */
    static size_t cache_cap  = 0;   /* capacity of cache array */

    /* hard‑coded initial values */
    const seq_t init[6] = { (seq_t)1, (seq_t)8, (seq_t)20,
                            (seq_t)32, (seq_t)50, (seq_t)80 };

    if (n == 0) return (seq_t)0;          /* undefined, harmless */

    if (n <= 6) return init[n-1];

    /* initialise cache on first call */
    if (cache_size == 0) {
        /* allocate enough room for at least the first six terms */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
        cache_size = 6;
    }

    /* grow cache if required */
    if (n > cache_size) {
        if ((size_t)n > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < (size_t)n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms up to n */
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            idx_t i = k - 1;                     /* 0‑based index */

            seq_t a1 = cache[i-1];   /* a(k-1) */
            seq_t a2 = cache[i-2];   /* a(k-2) */
            seq_t a3 = cache[i-3];   /* a(k-3) */
            seq_t a4 = cache[i-4];   /* a(k-4) */
            seq_t a5 = cache[i-5];   /* a(k-5) */

            /* compute 3*a1 - 4*a2 + 4*a3 - 3*a4 + a5
               using only positive intermediate values to avoid
               unsigned underflow */
            seq_t term3   = 3 * a1;
            seq_t term4   = 4 * a3;
            seq_t term5   = a5;
            seq_t sub12   = 4 * a2;
            seq_t sub34   = 3 * a4;
            seq_t sub     = sub12 + sub34;
            seq_t val     = term3 + term4 + term5;
            val           = val - sub;

            cache[i] = val;
        }
        cache_size = n;
    }

    return cache[n-1];
}
