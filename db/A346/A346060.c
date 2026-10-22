/*
 * A346060  G.f. A(x) satisfies: A(x) = 1 - x * A(x/(1 - x)) / (1 - x)^5.
 * https://oeis.org/A346060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n+1) = -∑_{k=0..n} binomial(n+4, k+4) * a(k) (OEIS
 *            A346060) (offset 0)
 * Range:     i8 n <= 3, i16 n <= 10, i32 n <= 15, i64 n <= 26, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0..cache_size-1] holds computed values */
static idx_t cache_size = 0;     /* current number of stored elements */

/* Ensure the cache has room for indices 0 … n   (size = n+1) */
static void ensure_cache_len(idx_t n)
{
    if (n + 1 <= cache_size) return;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) abort();              /* allocation failed */
    cache = tmp;
    cache_size = n + 1;
}

/* Compute a(n) with memoisation, safe for any call order */
seq_t a(idx_t n)
{
    /* initialise cache if necessary */
    if (cache_size == 0) {
        ensure_cache_len(0);       /* allocate cache[0] */
        cache[0] = (seq_t)1;       /* a(0) = 1 */
        cache_size = 1;
    }

    if (n == 0) return cache[0];

    /* If the cache already holds the value, return it */
    if (n < cache_size) return cache[n];

    /* Compute missing values from cache_size up to n */
    idx_t old_size = cache_size;   /* first index that needs computation */
    ensure_cache_len(n);           /* ensure storage for indices up to n */

    for (idx_t m = old_size; m <= n; ++m) {
        idx_t r = m - 1;           /* recurrence parameter n = m-1 */

        /* Compute binomial(r+4, 4) */
        seq_t binom = (seq_t)1;
        for (idx_t i = 1; i <= 4; ++i) {
            binom = binom * (seq_t)(r + 5 - i);
            binom = binom / (seq_t)i;
        }

        /* Sum over k = 0 .. r of binomial(r+4, k+4) * a(k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= r; ++k) {
            sum += binom * cache[k];
            if (k < r) {
                /* advance binom from C(r+4, k+4) to C(r+4, k+5) */
                binom = binom * (seq_t)(r - k);
                binom = binom / (seq_t)(k + 5);
            }
        }

        cache[m] = -sum;
    }

    /* Update cache_size to reflect that indices 0…n are now stored */
    cache_size = n + 1;
    return cache[n];
}
