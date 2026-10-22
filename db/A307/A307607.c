/*
 * A307607  a(n) = 1 + Sum_{d|n, d > 1} d^2*a(n/d).
 * https://oeis.org/A307607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{d|n, d>1} d^2 * a(n/d)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1151, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, calloc, abort */
#include <stddef.h>   /* size_t */

/* seq_t and idx_t are defined elsewhere in the build environment. */
extern seq_t a(idx_t n);

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Forward declaration of the recursive helper */
static seq_t compute_a(idx_t n);

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* behaviour below the offset is irrelevant */

    /* Ensure the cache can hold values up to index n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        /* Initialise newly allocated slots to zero (uncomputed flag) */
        for (size_t i = cache_len; i < new_len; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_len = new_len;
    }

    /* Return cached value if already computed */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Compute the value recursively */
    return compute_a(n);
}

/* Recursive computation of a(n) */
static seq_t compute_a(idx_t n)
{
    if (n == 1) { /* base case */
        cache[1] = (seq_t)1;
        return cache[1];
    }

    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 > (idx_t)1) { /* divisor d1 > 1 */
                seq_t val = a(n / d1);
                sum += (seq_t)d1 * d1 * val;
            }
            if (d2 != d1 && d2 > (idx_t)1) { /* distinct divisor d2 > 1 */
                seq_t val = a(n / d2);
                sum += (seq_t)d2 * d2 * val;
            }
        }
    }

    cache[n] = sum + (seq_t)1;
    return cache[n];
}
