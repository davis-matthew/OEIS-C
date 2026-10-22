/*
 * A366182  G.f. A(x) satisfies A(x) = 1/(1 - x)^3 + x*A(x)^3/(1 - x).
 * https://oeis.org/A366182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+6*k+2, n-k) * binomial(3*k,
 *            k)/(2*k+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* first binomial factor */
        seq_t b1 = binomial(n + 6 * k + 2, n - k);
        /* second binomial factor, divided by (2*k+1) */
        seq_t b2 = binomial(3 * k, k);
        b2 = b2 / (seq_t)(2 * k + 1);
        /* product of the two factors */
        seq_t term = seq_mul(b1, b2);
        /* accumulate */
        sum += term;
    }
    return sum;
}
