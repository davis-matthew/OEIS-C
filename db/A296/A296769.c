/*
 * A296769  Row sums of A296662.
 * https://oeis.org/A296769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((C(2n+4,n+1)*(n+3))/4) - 2^(2n+1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/pow2.h"

static inline seq_t binom_2n4_n1(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n + 1; ++i)
    {
        seq_t num = (seq_t)(2 * n + 4 - (i - 1));
        seq_t den = (seq_t)i;

        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        seq_t g2 = gcd(result, den);
        result /= g2;
        den /= g2;               /* den should now be 1 */

        result = result * num;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t bin = binom_2n4_n1(n);

    seq_t term = bin * (seq_t)(n + 3);
    term = term / (seq_t)4;

    seq_t pow_term = pow2((idx_t)(1 + 2 * n));

    return term - pow_term;
}
