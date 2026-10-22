/*
 * A330513  a(n) = a(n-1) + a(floor(n/4)), a(1)=a(2)=a(3) = 1.
 * https://oeis.org/A330513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(floor(n/4)), a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 53, u16 n <= 324, u32 n <= 3933, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization */
static seq_t *seq_cache = NULL;
static idx_t seq_cache_len = 0;   /* number of computed elements; indices 1..seq_cache_len are valid */

/* Compute A330513 */
seq_t a(idx_t n)
{
    if (n <= 3) {
        return (seq_t)1;
    }

    /* Ensure the cache is large enough and compute missing terms */
    if (seq_cache_len < n) {
        /* Determine new capacity, doubling until it exceeds n */
        size_t new_cap = seq_cache_len > 0 ? (size_t)seq_cache_len : 8;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }

        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();
        }
        seq_cache = tmp;

        /* Initialize the first three values if they are not yet set */
        if (seq_cache_len < 3) {
            for (idx_t i = seq_cache_len + 1; i <= 3; ++i) {
                seq_cache[i] = (seq_t)1;
            }
        }

        /* Compute new values up to n */
        for (idx_t i = seq_cache_len + 1; i <= n; ++i) {
            seq_t val;
            if (i <= 3) {
                val = (seq_t)1;
            } else {
                val = seq_cache[i - 1] + seq_cache[i >> 2];
            }
            seq_cache[i] = val;
        }

        seq_cache_len = n;
    }

    return seq_cache[n];
}
