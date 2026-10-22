/*
 * A320329  Row sums of A174790.
 * https://oeis.org/A320329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n - 2^n * n! + 2 * (2*n - 1)!/(n - 1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t term1 = 1;
    seq_t term2 = (seq_t)n;

    seq_t fact_n   = factorial(n);
    seq_t pow2_n   = pow2(n);
    seq_t term3    = pow2_n * fact_n;          /* 2^n * n! */

    /* product = n * (n+1) * ... * (2n-1) = (2n-1)! / (n-1)!  */
    seq_t product = 1;
    idx_t upper = n + n - 1;   /* 2*n - 1, safe for n <= 10000 */
    for (idx_t k = n; k <= upper; ++k)
    {
        product *= (seq_t)k;
    }
    seq_t term4 = 2 * product;              /* 2 * product */

    seq_t result = term1 + term2 - term3 + term4;
    return result;
}
