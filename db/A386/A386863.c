/*
 * A386863  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(3*n+1,k) *
 *          binomial(3*n-k-1,n-k).
 * https://oeis.org/A386863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A386863
   a(n) = Σ_{k=0..n} 2^(n-k) * binomial(3*n+1,k) * binomial(3*n-k-1,n-k)
   (binomial(m,k) is the ordinary binomial coefficient).  The routine
   computes the sum in linear time by iteratively updating the two
   binomial factors and halving the power of two at each step. */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Compute the nth term of A386863. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 */

    /* power2 = 2^n, then halved after each term */
    seq_t power2 = big_pow((seq_t)2, (seq_t)n);

    /* C(3n+1,0) = 1 */
    seq_t b = 1;

    /* C(3n-1,n) */
    seq_t m = binomial(3 * n - 1, n);

    seq_t result = b * m * power2;   /* term for k = 0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* Update C(3n+1,k) */
        b = (b * (seq_t)(3 * n + 2 - k)) / (seq_t)k;

        /* Update C(3n-k-1,n-k) */
        m = (m * (seq_t)(n - k + 1)) / (seq_t)(3 * n - k);

        /* Halve the power of two */
        power2 >>= 1;

        result += b * m * power2;
    }

    return result;
}
