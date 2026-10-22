/*
 * A360707  G.f.: Sum_{k>=0} (1 + k*x)^k * x^(3*k).
 * https://oeis.org/A360707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(k, n-3k) * k^(n-3k) (OEIS A360707)
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 37, u64 n <= 62, u128 n <= 106,
 *            big any n
 * Helpers:   binom_km
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom_km.h"

/* Forward declarations of types provided by the build */
typedef uint64_t idx_t; /* placeholder: actual type is defined by the build system */

/* Power of a seq_t base to a non‑negative idx_t exponent */
static inline seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return result;
}

/* --- Memoized sequence storage */
static seq_t *cache = NULL;        /* array of computed terms */
static idx_t cache_size = 0;       /* number of terms currently stored (+1 for last index) */
static idx_t cache_capacity = 0;   /* allocated capacity */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold a(n) */
    while (cache_capacity <= n) {
        cache_capacity = cache_capacity ? cache_capacity * 2 : 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                    (size_t)cache_capacity * sizeof(seq_t));
        if (!tmp) abort();       /* allocation failed */
        cache = tmp;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {            /* a(0) = 1 */
            cache[i] = 1;
            continue;
        }

        seq_t sum = 0;
        idx_t lower_k = (i + 3) / 4; /* ceil(i/4) */
        idx_t upper_k = i / 3;       /* floor(i/3) */

        for (idx_t k = lower_k; k <= upper_k; ++k) {
            idx_t m = i - 3 * k;            /* exponent and binomial upper index */
            seq_t bin = binom_km(k, m);
            seq_t pw  = pow_uint((seq_t)k, m);
            sum += bin * pw;
        }
        cache[i] = sum;
    }

    cache_size = n + 1;
    return cache[n];
}
