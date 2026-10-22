/*
 * A365550  The number of square coreful divisors of n.
 * https://oeis.org/A365550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e // n} floor(e/2) (a(p^e)=floor(e/2); if any
 *            exponent e = 1 the product is 0, and a(1)=1 )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* N = 1 has one square coreful divisor: {1}. */
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor 2 separately to avoid testing it in the loop */
    int e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e) {
        if (e < 2)          /* exponent 1 -> not powerful */
            return (seq_t)0;
        result *= (seq_t)(e >> 1);   /* e/2 for integer division */
    }

    /* Test odd divisors up to sqrt(m) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (e < 2)          /* exponent 1 -> not powerful */
                return (seq_t)0;
            result *= (seq_t)(e >> 1);
        }
    }

    /* If a prime factor remains, its exponent is 1 */
    if (m > 1)
        return (seq_t)0;

    return result;
}
