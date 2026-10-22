/*
 * A300443  Number of binary enriched p-trees of weight n.
 * https://oeis.org/A300443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 42, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=1+sum_{x=1}^{floor(n/2)} a(x)*a(n-x) */
#include <stdlib.h>
#include <stddef.h>

/* Cache of already computed terms */
static seq_t *cache          = NULL; /* array of a(0)..a(max) */
static idx_t cache_cap       = 0;    /* allocated size (in elements) */
static idx_t last_computed   = 0;    /* index of the largest computed term */

/* Ensure that the cache can hold terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Return the n-th term of A300443. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* Initialise a(0)=1 once. */
    if (cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
    }

    /* Compute missing terms up to n. */
    if (n > last_computed) {
        for (idx_t k = last_computed + 1; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            idx_t half = k >> 1;   /* floor(k/2) */
            for (idx_t x = 1; x <= half; ++x) {
                sum += cache[x] * cache[k - x];
            }
            cache[k] = (seq_t)1 + sum;
        }
        last_computed = n;
    }
    return cache[n];
}
