/*
 * A370248  Coefficient of x^n in the expansion of ( 1/(1-x) * (1+x^3)^2 )^n.
 * https://oeis.org/A370248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n,k) *
 *            binomial(2*n-3*k-1, n-3*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 3; ++k)
    {
        idx_t two_n = n + n;                            /* 2*n */
        seq_t c1   = binomial(two_n, k);
        idx_t rem  = two_n - 3 * k - 1;                 /* 2*n - 3*k - 1 */
        idx_t nmk  = n - 3 * k;                         /* n - 3*k */
        seq_t c2   = binomial(rem, nmk);
        sum += c1 * c2;
    }
    return sum;
}
