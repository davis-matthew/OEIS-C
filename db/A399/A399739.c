/*
 * A399739  The sum of the unitary divisors d of n such that n/d is a power of
 *          2.
 * https://oeis.org/A399739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n if n is odd * a(n) = n + n/2^v2(n) (the odd part of n)
 *            if n is even * This implements the description of sequence
 *            A399739: the sum of the * unitary divisors d of n for which n/d
 *            is a power of 2
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find v2(n), the exponent of 2 in n, and the odd part. */
    idx_t e = 0;
    idx_t temp = n;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++e;
    }

    /* If n is odd, the only unitary divisor with complementary power of 2
     * is n itself. */
    if ((n & 1) != 0) {
        return (seq_t)n;
    }

    /* n is even: contribute n and its odd part. */
    seq_t odd_part = (seq_t)(n >> e);   /* n / 2^e  */
    seq_t result = (seq_t)n + odd_part;
    return result;
}
