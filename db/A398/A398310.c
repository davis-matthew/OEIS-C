/*
 * A398310  Number of indecomposable closed walks of length n on the regular
 *          icosahedron based at a vertex.
 * https://oeis.org/A398310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 4*a(n-1) + 5*a(n-2) - 10*a(n-3) for n > 4, with a(0)=1,
 *            a(1)=0, a(2)=5, a(3)=10, a(4)=40
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_last = (idx_t)-1;    /* last index computed, -1 means none */

/* Ensure the cache contains values up to index n and compute any missing ones. */
static void seq_cache_ensure(idx_t n) {
    if (seq_cache_last == (idx_t)-1) {               /* first time */
        idx_t size = n + 1;
        if (size < 5) size = 5;
        seq_cache = (seq_t *)malloc(size * sizeof(seq_t));
        seq_cache[0] = 1;
        seq_cache[1] = 0;
        seq_cache[2] = 5;
        seq_cache[3] = 10;
        seq_cache[4] = 40;
        seq_cache_last = 4;
    } else if (seq_cache_last < n) {                /* grow cache */
        seq_cache = (seq_t *)realloc(seq_cache, (n + 1) * sizeof(seq_t));
    }
    /* compute missing terms */
    for (idx_t i = seq_cache_last + 1; i <= n; ++i) {
        seq_cache[i] =
            ((seq_t)4) * seq_cache[i - 1] +
            ((seq_t)5) * seq_cache[i - 2] -
            ((seq_t)10) * seq_cache[i - 3];
    }
    seq_cache_last = n;
}

/* Main sequence function requested by the build. */
seq_t a(idx_t n) {
    if (n <= 4) {
        static const seq_t base[5] = {1, 0, 5, 10, 40};
        return base[n];
    }
    seq_cache_ensure(n);
    return seq_cache[n];
}
