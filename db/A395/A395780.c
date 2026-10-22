/*
 * A395780  Expansion of (Sum_{k>=0} binomial(3*k+1,k) * x^k)^4.
 * https://oeis.org/A395780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 2^k * binomial(k+2,2) * binomial(3*n+7,n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t k;
    for (k = 0; k <= n; ++k) {
        /* 2^k */
        seq_t term = pow2(k);

        /* binomial(k+2,2) = (k+2)(k+1)/2 */
        seq_t c1 = ((seq_t)(k + 2) * (k + 1)) / 2;
        term *= c1;

        /* binomial(3*n+7, n-k) */
        idx_t upper = 3 * n + 7;
        idx_t lower = n - k;
        term *= binom(upper, lower);

        result += term;
    }
    return result;
}
