/*
 * A351407  a(1) = 1; a(n+1) = Sum_{d|n} (-1)^(n/d) * a(d).
 * https://oeis.org/A351407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n+1) = ∑_{d|n} (-1)^(n/d) * a(d) */
#include <stdlib.h>

/* External integer types supplied by the build system */
typedef seq_t seq_t;
typedef idx_t idx_t;

/* Memoised cache of computed terms (1‑indexed) */
static seq_t *cache   = NULL;   /* cache[0] unused */
static size_t cache_cap = 0;    /* allocated size of cache array */
static idx_t  cache_max  = 0;   /* largest n that has been computed */

/* Ensure that cache contains values up to and including n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max) return;          /* already available */

    /* First allocation */
    if (!cache) {
        size_t init_cap = 64;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        /* cache[1] = a(1) */
        cache[0] = 0;                    /* dummy, never used */
        cache[1] = (seq_t)1;
        cache_cap = init_cap;
        cache_max  = 1;
    }

    /* Grow the array if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        idx_t m = k - 1;          /* n in the recurrence */
        seq_t sum = (seq_t)0;

        for (idx_t i = 1; i <= m / i; ++i) {
            if (m % i == 0) {
                idx_t d1 = i;
                idx_t d2 = m / i;

                seq_t term1 = cache[(size_t)d1];
                if ((m / d1) & 1) term1 = -term1;
                sum += term1;

                if (d2 != d1) {
                    seq_t term2 = cache[(size_t)d2];
                    if ((m / d2) & 1) term2 = -term2;
                    sum += term2;
                }
            }
        }
        cache[(size_t)k] = sum;
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n < 1 */
    ensure_cache(n);
    return cache[(size_t)n];
}
