/*
 * A385085  a(n) = 2 * (3*n+2)^(n-1).
 * https://oeis.org/A385085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (3*n+2)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */
#include "../utils/mul_small.h" /* mul_small(seq_t x, unsigned int m) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;            /* a(0) = 1 */

    /* base = 3*n + 2 */
    seq_t base = (seq_t)3 * (seq_t)n + (seq_t)2;

    /* exponent = n-1 */
    seq_t exp = (seq_t)(n - 1);

    /* Compute  (3*n+2)^(n-1) and then multiply by 2 */
    seq_t result = big_pow(base, exp);
    result = mul_small(result, 2U);

    return result;
}
