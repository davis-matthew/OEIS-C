/*
 * A254408  a(n) = 2*n^2*binomial(2*n,n)^2, a closed form for a double
 *          binomial sum involving absolute values.
 * https://oeis.org/A254408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n^2*binomial(2*n,n)^2, a(0)=0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute C(2n, n) */
    seq_t comb = binomial(2 * n, n);

    /* compute 2 * n^2 * comb^2 */
    seq_t result = comb * comb;
    seq_t n_sq = (seq_t)n * (seq_t)n;
    result = result * (seq_t)2 * n_sq;

    return result;
}
