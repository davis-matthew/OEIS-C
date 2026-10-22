/*
 * A360820  a(n) = Sum_{k=0..n} binomial(n, k)*2^(n^2-k*(n-k)).
 * https://oeis.org/A360820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * 2^(n^2 - k*(n-k))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;                     /* running total of the sum */

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial coefficient C(n,k) */
        seq_t bin = binomial(n, k);

        /* exponent e = n^2 - k*(n-k)  (computed in idx_t to avoid overflow of seq_t) */
        idx_t exp = (idx_t)n * (idx_t)n - (idx_t)k * (idx_t)(n - k);

        /* term = C(n,k) * 2^exp */
        seq_t term = bin * pow2(exp);

        /* accumulate */
        sum += term;
    }

    return sum;
}
