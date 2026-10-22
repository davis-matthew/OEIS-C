/*
 * A360759  a(n) = Sum_{d|n} d^(d+n/d) * binomial(d,n/d).
 * https://oeis.org/A360759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d+n/d) * binomial(d,n/d)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t k = n / d;                     /* complementary divisor */
            seq_t exp = (seq_t)d + (seq_t)k;
            seq_t term = big_pow((seq_t)d, exp);
            term = seq_mul(term, binomial(d, k));
            result += term;

            if (d != k) {                         /* add the symmetric divisor */
                exp = (seq_t)k + (seq_t)d;         /* same exponent, but keep symmetry */
                term = big_pow((seq_t)k, exp);
                term = seq_mul(term, binomial(k, d));
                result += term;
            }
        }
    }
    return result;
}
