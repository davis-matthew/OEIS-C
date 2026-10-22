/*
 * A350387  a(n) is the sum of the odd exponents in the prime factorization of
 *          n.
 * https://oeis.org/A350387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of odd exponents in the prime factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t t = n;
    seq_t sum = 0;

    /* factor 2 */
    if ((t & 1) == 0) {
        idx_t exp = 0;
        while ((t & 1) == 0) {
            t >>= 1;
            exp++;
        }
        if (exp & 1) sum += exp;
    }

    /* odd factors */
    for (idx_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            idx_t exp = 0;
            while (t % p == 0) {
                t /= p;
                exp++;
            }
            if (exp & 1) sum += exp;
        }
    }

    /* remaining prime factor with exponent 1 */
    if (t > 1) sum += 1;

    return sum;
}
