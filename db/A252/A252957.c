/*
 * A252957  Number of (n+2)X(4+2) 0..4 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A252957
    a(n) = 5*a(n-1) - 6*a(n-2) + 5*a(n-3) - 40*a(n-4) + 57*a(n-5) + 67*a(n-6)
          - 110*a(n-7) + 15*a(n-8) - 415*a(n-9) + 1040*a(n-10) - 861*a(n-11)
          + 314*a(n-12) - 175*a(n-13) + 147*a(n-14) - 42*a(n-15)   (n > 17)
    Initial values (n = 1 … 17) are used for the recurrence. */

#include <stdlib.h>                /* size_t, malloc, realloc */
#include "../utils/mul_small.h"     /* seq_t mul_small(seq_t, unsigned int) */

/* Base values for n = 1 … 17. */
static const long long init_vals[17] = {
    603LL, 249LL, 369LL, 674LL, 1242LL, 2201LL, 4275LL, 8202LL,
    15026LL, 29799LL, 58371LL, 108338LL, 217140LL, 428327LL,
    803859LL, 1614920LL, 3205212LL
};

/* Recurrence coefficients as listed in the OEIS entry. */
static const int coeffs[15] = {
     5, -6,  5, -40,  57,  67, -110,  15, -415, 1040,
   -861, 314, -175, 147, -42
};

/* Cached sequence values. cache[0] holds a(1). */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_max  = 0;   /* largest n already computed (1‑based) */

/* Ensure that cache contains values up to and including n. */
static void ensure_cache(size_t n)
{
    if (n <= cache_max) return;                /* nothing to do */

    /* Allocate or extend the cache. */
    if (cache == NULL) {
        cache_size = (n > 17) ? n : 17;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    } else if (n > cache_size) {
        cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
        cache_size = n;
    }

    /* Fill the first 17 positions if they are not yet set. */
    if (cache_max < 17) {
        size_t last = (cache_max < 17) ? 17 : cache_max;
        for (size_t i = cache_max; i < last; ++i) {
            cache[i] = (seq_t)init_vals[i];
        }
    }

    /* Compute new terms using the recurrence. */
    for (size_t i = cache_max; i < n; ++i) {   /* i is 0‑based index = n-1 */
        if (i < 17) continue;                 /* already set */
        seq_t sum = (seq_t)0;
        for (int j = 0; j < 15; ++j) {
            seq_t prev = cache[i - j - 1];            /* a(i-j) */
            seq_t term = mul_small(prev,
                                   (unsigned int)(coeffs[j] >= 0 ?
                                                   coeffs[j] :
                                                   -coeffs[j]));
            if (coeffs[j] < 0)
                sum -= term;
            else
                sum += term;
        }
        cache[i] = sum;
    }

    cache_max = n;
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* undefined for n < 1 */

    /* Convert idx_t to size_t safely (n is always >=1). */
    size_t nn = (size_t)n;
    ensure_cache(nn);
    return cache[nn - 1];
}
