/*
 * A339240  a(n) = n*2^(2*n-2) + n*binomial(2*n,n)/2.
 * https://oeis.org/A339240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*2^(2*n-2) + n*binomial(2*n,n)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t exp = 2 * n - 2;          /* exponent for 2^(2n-2) */
    seq_t term1 = (seq_t)n * pow2(exp);     /* n * 2^(2n-2) */

    seq_t binom_val = binomial(2 * n, n);  /* central binomial coefficient */
    seq_t term2 = ((seq_t)n * binom_val) / (seq_t)2; /* n * binom(2n,n)/2 */

    return term1 + term2;
}
