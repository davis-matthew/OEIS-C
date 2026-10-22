/*
 * A391689  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(k+2,2) * binomial(k,n-k)^2.
 * https://oeis.org/A391689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(k+2,2) * binomial(k,
 *            n-k)^2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Multiply two seq_t values.  The builtin multiplication is used; if seq_t is
   a 64‑bit unsigned type this may overflow, but that is acceptable because
   the sequence is only required to be exact when the result actually fits
   in seq_t. */
static inline seq_t mul_seq(seq_t x, seq_t y)
{
    return x * y;
}

/* Compute a(n) from the definition.  This implementation uses only
   integer arithmetic and the helper functions provided in the utilities
   directory.  No memoisation or caching is performed, so the function
   works correctly for calls in any order. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* Power of two: 2^(n-k) */
        seq_t p2 = pow2(n - k);

        /* binomial(k+2, 2) = (k+2)(k+1)/2 */
        seq_t binom_k2 = mul_seq((seq_t)(k + 2), (seq_t)(k + 1)) / (seq_t)2;

        /* binomial(k, n-k) */
        seq_t binom_knk = binomial(k, n - k);

        /* term = 2^(n-k) * binomial(k+2,2) * binomial(k, n-k)^2 */
        seq_t term = mul_seq(p2, mul_seq(binom_k2, mul_seq(binom_knk, binom_knk)));

        sum += term;
    }
    return sum;
}
