/*
 * A342380  Expansion of e.g.f. (exp(x)-1)*(exp(x) - x^4/24 - x^3/6 - x^2/2 -
 *          x - 1).
 * https://oeis.org/A342380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n -
 *            [binomial(n,0)+binomial(n,1)+binomial(n,2)+binomial(n,3)+binomial(n,4)+
 *            (n>4?1:0)]
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = pow2(n);
    seq_t sum = 0;
    idx_t limit = (n < 4 ? n : 4);
    for (idx_t i = 0; i <= limit; ++i)
        sum += binomial(n, i);
    if (n > 4)
        sum += 1;
    return result - sum;
}
