/*
 * A351659  G.f. A(x) satisfies: A(x) = 1 - x * A(x/(1 - x)^2) / (1 - x)^2.
 * https://oeis.org/A351659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{k=0..n-1} binomial(n+k, n-k-1) * a(k); a(0) = 1
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   binomial_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial_int.h"

/* cache for computed values */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_max = (size_t)-1;   /* no values computed yet */

seq_t a(idx_t n) {
    /* ensure at least n+1 capacity */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;   /* out of memory, return 0 as error */
        seq_cache = new_cache;
        cache_cap = new_cap;
    }

    if (cache_max == (size_t)-1) {
        /* first call: initialize a(0) */
        seq_cache[0] = 1;
        cache_max = 0;
    }

    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t bin = binomial_int(i + k, i - k - 1);
                sum += bin * seq_cache[k];
            }
            seq_cache[i] = -sum;
        }
        cache_max = n;
    }

    return seq_cache[(size_t)n];
}
