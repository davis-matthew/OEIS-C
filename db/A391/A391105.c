/*
 * A391105  a(n) = binomial(4*n,n) - binomial(3*n,n).
 * https://oeis.org/A391105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(4*n,n) - binomial(3*n,n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;
    if (K > N - K) K = N - K;          /* use symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num = (seq_t)(N - K) + (seq_t)i;   /* (N-K+i) */
        seq_t den = (seq_t)i;

        /* cancel common factors with result */
        seq_t g = gcd(result, den);
        result /= g;
        den /= g;

        /* cancel common factors between numerator and remaining denom */
        g = gcd(num, den);
        num /= g;
        den /= g;   /* den is now 1 */

        result *= num;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* binomial(0,0)-binomial(0,0)=0 */

    seq_t term1 = binom(4 * n, n);
    seq_t term2 = binom(3 * n, n);
    return term1 - term2;
}
