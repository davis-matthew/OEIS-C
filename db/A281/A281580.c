/*
 * A281580  a(n) = binomial(9*n, n-9).
 * https://oeis.org/A281580
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(9*n, n-9)
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 14, u64 n <= 21, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static inline seq_t binom_mult(idx_t N, idx_t K)
{
    if (K > N) return (seq_t)0;
    if (K == 0 || K == N) return (seq_t)1;
    if (K > N - K) K = N - K;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num = (seq_t)(N - K + i);
        seq_t den = (seq_t)i;
        seq_t g;

        /* Reduce the current result by the denominator */
        g = gcd(result, den);
        result /= g;
        den /= g;

        /* Reduce the numerator by any remaining denominator */
        if (den != (seq_t)1)
        {
            g = gcd(num, den);
            num /= g;
            den /= g;
        }

        /* Multiply and, if necessary, divide */
        result *= num;
        if (den != (seq_t)1) result /= den;   /* should normally be 1 */
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 9) return (seq_t)0;                 /* offset */
    return binom_mult((idx_t)(9 * n), (idx_t)(n - 9));
}
