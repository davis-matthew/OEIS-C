/*
 * A391778  a(n) = n*(n-1)*(n- 2)^2*2^(n-4)/3.
 * https://oeis.org/A391778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)^2*2^(n-4)/3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    /* n = 3 gives the only special case for the negative exponent 2^-1 */
    if (n == 3)
        return (seq_t)1;

    /* For n >= 4, 2^(n-4) is an integer */
    seq_t prod = (seq_t)n * (seq_t)(n - 1);
    prod *= (seq_t)(n - 2);
    prod *= (seq_t)(n - 2);
    prod *= pow2(n - 4);
    prod /= (seq_t)3;
    return prod;
}
