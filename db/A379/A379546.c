/*
 * A379546  Expansion of (1/x) * Series_Reversion( x / ( (1+x)^2 * (1+2*x)^3 )
 *          ).
 * https://oeis.org/A379546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A379546: a(n) = (1/(n+1)) * Σ_{k=0..n} 2^k * binomial(3*(n+1),k) * binomial(2*(n+1),n-k) */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

static seq_t compute_a_value(idx_t n)
{
    idx_t N = n + 1;   /* n+1 */

    /* 64‑bit (or smaller) build uses __uint128_t for the intermediate arithmetic. */
#ifdef __cplusplus
    /* Arbitrary‑precision build: use the arbitrary‑precision utilities. */
    seq_t sum   = (seq_t)0;
    seq_t pow2  = (seq_t)1;
    seq_t binom1 = (seq_t)1;
    seq_t binom2 = binomial(2 * N, n);

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = seq_mul(pow2, binom1);
        term       = seq_mul(term, binom2);
        sum        = sum + term;          /* addition is provided for all seq_t types */

        if (k == n) break;

        pow2  = pow2 * 2;
        binom1 = seq_mul(binom1, (seq_t)(3 * N - k)) / (seq_t)(k + 1);
        idx_t m     = n - k;
        binom2 = seq_mul(binom2, (seq_t)m) / (seq_t)(2 * N - m + 1);
    }

    return sum / (seq_t)N;
#else
    /* 64‑bit (or smaller) build.  __uint128_t protects intermediate products. */
    __uint128_t sum   = 0;
    __uint128_t pow2  = 1;
    __uint128_t binom1 = 1;
    __uint128_t binom2 = (__uint128_t)binomial(2 * N, n);

    for (idx_t k = 0; k <= n; ++k) {
        sum += pow2 * binom1 * binom2;

        if (k == n) break;

        pow2 <<= 1;                        /* pow2 *= 2 */
        binom1 = binom1 * (3 * N - k) / (k + 1);
        idx_t m      = n - k;
        binom2 = binom2 * m / (2 * N - m + 1);
    }

    return (seq_t)(sum / N);
#endif
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    /* Cache of already calculated terms */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;

    if (n >= cache_size) {
        size_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            exit(EXIT_FAILURE);          /* allocation failure – terminate safely */
        }
        cache = tmp;
        for (size_t i = cache_size; i <= n; ++i) {
            cache[i] = compute_a_value((idx_t)i);
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}
