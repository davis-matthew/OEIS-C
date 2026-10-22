/*
 * A388428  Expansion of e.g.f. (1 + x)*(1 + x^2/2)*cosh(x).
 * https://oeis.org/A388428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2)+1 if n is even, otherwise a(n) =
 *            3*binomial(n,3)+n
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {
        /* n is even */
        return binom(n, 2) + 1;
    } else {
        /* n is odd */
        return (seq_t)3 * binom(n, 3) + n;
    }
}
