/*
 * A360588  Expansion of e.g.f. (exp(x)-1)^2*(x+x^2/2).
 * https://oeis.org/A360588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*2^(n-1) + binomial(n,2)*(2^(n-2)-2) - 2*n for n >= 3
 *            a(n) = 0 for n < 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t n_seq = (seq_t)n;

    /* term1 = n * 2^(n-1) */
    seq_t term1 = n_seq * pow2(n - 1);

    /* binom = binomial(n, 2) = n*(n-1)/2 */
    seq_t binom = n_seq * (n_seq - 1);
    binom /= 2;

    /* term2 = binom * (2^(n-2) - 2) */
    seq_t term2 = binom * (pow2(n - 2) - 2);

    /* subtract 2*n */
    seq_t minus = 2 * n_seq;

    return term1 + term2 - minus;
}
