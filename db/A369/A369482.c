/*
 * A369482  Expansion of (1/x) * Series_Reversion( x / ((1+x)^2 * (1+x+x^3))
 *          ).
 * https://oeis.org/A369482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(n+1,k) *
 *            binomial(3*n-k+3, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* Use the utility binomial if available, otherwise use a 128‑bit fallback */
#include "../utils/binomial.h"

/* 128‑bit binomial for n up to 63 (all binomial values fit in 128 bits) */
static seq_t binom_u128(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (unsigned __int128)(n - k + i);
        r /= i;
    }
    return (seq_t)r;
}

/* Dispatch to the fast routine when the result fits in 128 bits,
   otherwise fall back to the generic (big integer) implementation. */
static seq_t choose(idx_t n, idx_t k)
{
    /* 63 is the largest n for which binomial(n,k) < 2^128 for all k. */
    if (sizeof(seq_t) <= 8 && n <= 63)
        return binom_u128(n, k);
    return binomial(n, k);
}

seq_t a(idx_t n)
{
    idx_t k_max = n / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= k_max; ++k) {
        seq_t c1 = choose(n + 1, k);
        seq_t c2 = choose(3 * n - k + 3, n - 3 * k);

        /* Multiply with 128‑bit to avoid overflow for the 64‑bit build. */
        unsigned __int128 prod = (unsigned __int128)c1 * (unsigned __int128)c2;
        sum += (seq_t)prod;
    }

    return sum / (seq_t)(n + 1);
}
