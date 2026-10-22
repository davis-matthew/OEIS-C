/*
 * A385047  The sum of the unitary divisors of n that are powers of 2.
 * https://oeis.org/A385047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 + 2^v2(n) if n is even, otherwise a(n) = 1 where v2(n)
 *            is the exponent of the highest power of two dividing n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* odd numbers have only the unitary divisor 1 that is a power of 2 */
    if (n & 1ULL)
        return (seq_t)1;

    /* find the exponent of the highest power of two dividing n */
    idx_t e = 0;
    idx_t t = n;
    while ((t & 1ULL) == 0) {
        ++e;
        t >>= 1;
    }

    /* return 1 + 2^e */
    return (seq_t)1 + (seq_t)(1ULL << e);
}
