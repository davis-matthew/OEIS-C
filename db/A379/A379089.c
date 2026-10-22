/*
 * A379089  G.f. A(x) satisfies A(x) = (1 + x*A(x)^2) * (1 + x^3*A(x)^7).
 * https://oeis.org/A379089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} binomial(2*n+k+1,k) *
 *            binomial(2*n+k+1,n-3*k)/(2*n+k+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m = 2 * n + k + 1;          /* m = 2*n + k + 1 */
        seq_t bin1 = binomial(m, k);     /* binomial(2*n+k+1, k) */
        idx_t nk = n - 3 * k;             /* nk = n - 3*k >= 0   */
        seq_t bin2 = binomial(m, nk);    /* binomial(2*n+k+1, n-3k) */
        seq_t term = bin1 * bin2 / (seq_t)m;
        result += term;
    }

    return result;
}
