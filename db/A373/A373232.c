/*
 * A373232  Relative of Hofstadter Q-sequence: a(n) = 0 for n <= 0, a(n) = n
 *          for 1 <= n <= 12; a(n) = a(n-a(n-1)) + a(n-a(n-2)) + a(n-a(n-3))
 *          for n > 12.
 * https://oeis.org/A373232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for 1 <= n <= 12; a(n) = a(n-a(n-1)) + a(n-a(n-2)) +
 *            a(n-a(n-3)) for n > 12
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* memoization array */
static size_t cache_cap = 0;          /* current capacity of cache   */
static idx_t last_computed = 12;      /* largest n whose a(n) is stored */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return 0;
    if (n <= 12) return (seq_t)n;

    /* Initialise cache on first use */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
        for (idx_t k = 1; k <= 12; ++k) cache[k-1] = (seq_t)k;
    }

    /* Grow cache if needed */
    if (n > last_computed) {
        if ((size_t)n > cache_cap) {
            size_t new_cap = cache_cap;
            if (new_cap == 0) new_cap = 128;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;          /* allocation failed */
            memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms up to n */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* Previous values */
            long long a_im1 = (long long)cache[i-2]; /* a(i-1) */
            long long a_im2 = (long long)cache[i-3]; /* a(i-2) */
            long long a_im3 = (long long)cache[i-4]; /* a(i-3) */

            seq_t val1 = 0, val2 = 0, val3 = 0;

            long long idx1 = (long long)i - a_im1;
            if (idx1 > 0) val1 = cache[(size_t)idx1 - 1];
            long long idx2 = (long long)i - a_im2;
            if (idx2 > 0) val2 = cache[(size_t)idx2 - 1];
            long long idx3 = (long long)i - a_im3;
            if (idx3 > 0) val3 = cache[(size_t)idx3 - 1];

            cache[i-1] = val1 + val2 + val3;
        }
        last_computed = n;
    }

    return cache[n-1];
}
