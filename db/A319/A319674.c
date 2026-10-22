/*
 * A319674  a(n) = 1 + 2 + 3 - 4 - 5 - 6 + 7 + 8 + 9 - 10 - 11 - 12 + ... -
 *          (up to n).
 * https://oeis.org/A319674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, easy
 * Formula:   a(n) = Sum_{i=1..n} i * (-1)^floor((i-1)/3)
 * Range:     i8 n <= 86, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              i.e. add three terms, then subtract three terms, etc. */
/* Offset: 1 (a(1) is the first term) */

#include <stdlib.h>

/* Cache of already computed values; cache[0] holds the value for n=0. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* n should be >= 1; for safety return 0 for n==0. */
    if (n == 0) return 0;

    /* Return cached value if already computed. */
    if (n <= cache_size) return cache[n];

    /* Grow cache to support indices up to n. */
    idx_t new_size = n + 1;                     /* indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {                                 /* allocation failed */
        return 0;
    }
    cache = tmp;

    /* Ensure that cache[0] exists and is initialized to zero. */
    if (cache_size == 0) {
        cache[0] = 0;
    }

    /* Compute new values incrementally. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        idx_t block  = (i - 1) / 3;                     /* block number (0‑based)   */
        seq_t sign   = ((block & 1) == 0) ? (seq_t)1 : (seq_t)-1;   /* +1 for even block, -1 otherwise */
        cache[i] = cache[i - 1] + sign * (seq_t)i;
    }

    cache_size = n;
    return cache[n];
}
