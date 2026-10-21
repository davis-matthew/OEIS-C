/*
 * A293270  a(n) = n^n*binomial(2*n-1, n).
 * https://oeis.org/A293270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n*binomial(2*n-1, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t pow_n = big_pow((seq_t)n, (seq_t)n);          /* compute n^n */
    seq_t bin    = binomial(2*n - 1, n);                /* compute C(2n-1, n) */
    return pow_n * bin;                                 /* n^n * C(2n-1,n) */
}
