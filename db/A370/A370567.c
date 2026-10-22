/*
 * A370567  Size of the group Q_7*_/(Q_7*)^n, where Q_7 is the field of 7-adic
 *          numbers.
 * https://oeis.org/A370567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * 7^v7(n) * gcd(6, n / 7^v7(n)) (where v7(n) is the
 *            exponent of 7 in the prime factorization of n)
 * Range:     u8 n <= 20, u16 n <= 293, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for n=0, but offset starts at 1 */
    seq_t original = (seq_t)n;
    seq_t tmp = original;
    seq_t pow7 = 1;

    /* factor out all powers of 7 */
    while ((tmp % 7) == 0) {
        tmp /= 7;
        pow7 *= 7;
    }

    /* extra factor from primes 2 and/or 3 */
    seq_t g = gcd((seq_t)6, tmp);

    /* compute the result: n * 7^e * gcd(6, n') */
    return original * pow7 * g;
}
