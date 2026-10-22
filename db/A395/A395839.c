/*
 * A395839  G.f. 10*C(x)^2 - C(x), where C(x) = 1 + x*C(x)^2 is the g.f. of
 *          the Catalan numbers (A000108).
 * https://oeis.org/A395839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*binomial(2*n+2,n)/(n+1) - binomial(2*n+1,n)/(2*n+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t k1 = 2 * n + 2;
    idx_t k2 = n;

    seq_t comb1 = binomial(k1, k2);          /* binomial(2n+2, n)  */
    seq_t comb2 = binomial(2 * n + 1, n);    /* binomial(2n+1, n)  */

    seq_t denom1 = (seq_t)(n + 1);
    seq_t term1  = (comb1 * (seq_t)10) / denom1;

    seq_t denom2 = (seq_t)(2 * n + 1);
    seq_t term2  = comb2 / denom2;

    return term1 - term2;
}
