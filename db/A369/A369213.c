/*
 * A369213  Expansion of (1/x) * Series_Reversion( x / ((1+x)^4+x^2) ).
 * https://oeis.org/A369213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/2⌋} binomial(n+1,k) *
 *            binomial(4n-4k+4,n-2k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes term a(n) of OEIS A369213. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute binomial coefficient using 128‑bit arithmetic to avoid overflow
   for the values needed in this sequence.  The result fits in __uint128_t
   for all n where the final a(n) fits in seq_t (i.e., n up to about 30). */
static __uint128_t binom128(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    __uint128_t result = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        result = result * (N - K + i) / i;
    }
    return result;
}

/* Memoisation storage */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure cache is large enough */
    if (n >= cache_size)
    {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        cache = tmp;
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    /* For n=0, the value is 1 */
    if (n == 0) return 1;

    /* Return cached value if already computed */
    if (cache[n] != 0) return cache[n];

    /* Compute a(n) via the given formula */
    __uint128_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        __uint128_t b1 = binom128(n + 1, k);
        idx_t big   = 4 * n - 4 * k + 4;
        idx_t small = n - 2 * k;
        __uint128_t b2 = binom128(big, small);
        sum += b1 * b2;                 /* 128‑bit product fits for the n values
                                            that produce a 64‑bit result */
    }

    __uint128_t denom = ( __uint128_t)(n + 1);
    __uint128_t result128 = sum / denom;

    /* Store in cache and return */
    cache[n] = (seq_t)result128;
    return cache[n];
}
