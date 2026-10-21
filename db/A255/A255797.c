/*
 * A255797  Number of (n+2) X (4+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 1 and no antidiagonal sum 2 and no row sum 0 and no
 *          column sum 3.
 * https://oeis.org/A255797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-6) – 2*a(n-12) (for n > 15) Base values (a(1) …
 *            a(15)): 409, 262, 198, 200, 268, 263, 384, 336, 326, 344, 476,
 *            471, 708, 608, 598
 * Range:     u8 none, u16 n <= 54, u32 n <= 150, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static table of the first 15 terms */
static const seq_t base[16] = {
    (seq_t)0,      /* unused, a(0) */
    (seq_t)409,    /* a(1) */
    (seq_t)262,    /* a(2) */
    (seq_t)198,    /* a(3) */
    (seq_t)200,    /* a(4) */
    (seq_t)268,    /* a(5) */
    (seq_t)263,    /* a(6) */
    (seq_t)384,    /* a(7) */
    (seq_t)336,    /* a(8) */
    (seq_t)326,    /* a(9) */
    (seq_t)344,    /* a(10) */
    (seq_t)476,    /* a(11) */
    (seq_t)471,    /* a(12) */
    (seq_t)708,    /* a(13) */
    (seq_t)608,    /* a(14) */
    (seq_t)598     /* a(15) */
};

/* cached values of the sequence */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently cached (maximum index) */
static size_t cache_cap = 0;   /* allocated capacity (number of elements) */

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0; /* undefined for n < 1, return 0 as sentinel */

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 16;                         /* allocate for indices 0..15 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                    /* allocation failure */
        /* copy the base terms into the cache */
        for (idx_t i = (idx_t)1; i <= (idx_t)15; ++i)
            cache[(size_t)i] = base[(size_t)i];
        cache_len = 15;
    }

    /* return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n];

    /* grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute intermediate terms up to n */
    for (idx_t k = (idx_t)(cache_len + 1); k <= n; ++k) {
        if (k <= (idx_t)15)
            cache[(size_t)k] = base[(size_t)k];
        else
            cache[(size_t)k] = (seq_t)3 * cache[(size_t)(k - 6)]
                               - (seq_t)2 * cache[(size_t)(k - 12)];
    }

    cache_len = (size_t)n;
    return cache[(size_t)n];
}
