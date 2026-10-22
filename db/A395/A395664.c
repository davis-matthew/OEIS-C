/*
 * A395664  Expansion of g^5/(4-3*g)^4, where g = 1+x*g^4 is the g.f. of
 *          A002293.
 * https://oeis.org/A395664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * binomial(k+2,2) * binomial(4*n+4,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* use seq_t for arithmetic */
    seq_t result = 0;
    seq_t pow3   = 1;

    /* initial binomial coefficient C(4*n+4, n) */
    idx_t upper = 4 * n + 4;
    idx_t r     = n;                 /* current argument in C(upper, r) */
    seq_t c     = binom(upper, r);   /* binom returns seq_t */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(k+2,2) = (k+2)*(k+1)/2 */
        seq_t coeff = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;

        /* term = 3^k * coeff * C(4*n+4, n-k) */
        seq_t term = pow3 * coeff * c;
        result += term;

        if (k == n) break;            /* finished all terms */

        /* update r and binomial coefficient for next k (k+1) */
        r--;                         /* r = n - (k+1) */
        /* C(upper, r) = C(upper, r+1) * (r+1) / (upper - r)  */
        seq_t numerator   = c * (seq_t)(r + 1);
        seq_t denominator = (seq_t)(upper - r);
        c = numerator / denominator;

        /* next power of 3 */
        pow3 *= 3;
    }

    return result;
}
