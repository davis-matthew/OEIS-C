/*
 * A371605  Expansion of e.g.f. 1 / (1 - x + x^2/2 - x^3/3).
 * https://oeis.org/A371605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = a(2) = 1 a(n) = n * a(n-1) - n * (n-1) * a(n-2) /
 *            2 + n * (n-1) * (n-2) * a(n-3) / 3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;          /* cached values of the sequence          */
static size_t cache_cap = 0;         /* current allocated size of the cache   */
static idx_t computed = 2;           /* largest index that has been computed */

seq_t a(idx_t n)
{
    /* Return the three initial values immediately. */
    if (n <= 2) {
        return 1;
    }

    /* Initialize the cache on first use. */
    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = 1;
        computed = 2;
    }

    /* Grow the cache so it can hold the requested index. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t term1 = (seq_t)i * cache[(size_t)(i - 1)];
            seq_t term2 = (seq_t)i * (seq_t)(i - 1) * cache[(size_t)(i - 2)];
            term2 /= 2;
            seq_t term3 = (seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2)
                          * cache[(size_t)(i - 3)];
            term3 /= 3;
            cache[(size_t)i] = term1 - term2 + term3;
        }
        computed = n;
    }

    return cache[(size_t)n];
}
