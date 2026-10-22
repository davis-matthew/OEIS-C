/*
 * A381292  Expansion of g.f. hypergeom([1/2, 3/4, 5/4], [1, 3/2], 256*x).
 * https://oeis.org/A381292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2n-1) * binom(4n+2, 2n+1) * binom(2n, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 is a special case because the factor 2^(2n-1) would be 2^-1 */
    if (n == 0ULL) {
        return (seq_t)1;
    }

    /* Compute 2^(2n-1) */
    idx_t exponent = 2 * n - 1;            /* 2n-1 >= 1 for n>=1 */
    seq_t power_of_two = pow_uint(2, exponent);

    /* Compute the two binomial coefficients */
    seq_t c1 = binom(4 * n + 2, 2 * n + 1);   /* C(4n+2 , 2n+1) */
    seq_t c2 = binom(2 * n,     n );           /* C(2n   ,  n   ) */

    /* Multiply the factors */
    return power_of_two * c1 * c2;
}
