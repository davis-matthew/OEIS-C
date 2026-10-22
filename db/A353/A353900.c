/*
 * A353900  a(n) is the sum of divisors of n whose exponents in their prime
 *          factorizations are all powers of 2 (A138302).
 * https://oeis.org/A353900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e of (1 + sum_{k=0..⌊log₂e⌋}
 *            p^(2^k))
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t result = 1;          /* intermediate type wide enough for n≤10000 */
    uint64_t t = n;

    /* trial division */
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p != 0) continue;

        /* count exponent of p */
        uint64_t exp = 0;
        while (t % p == 0) {
            t /= p;
            ++exp;
        }

        /* compute a(p^exp) as 1 + p + p² + p⁴ + …  (powers of 2) */
        uint64_t sum = 1;           /* leading 1 */
        uint64_t pow = p;           /* current p^(2^k) */
        uint64_t cur = 1;           /* current exponent 2^k */

        while (cur <= exp) {
            sum += pow;
            if (cur > exp / 2) break;  /* next power would exceed exp */
            pow = pow * pow;           /* square to get next power */
            cur <<= 1;                 /* next exponent 2^(k+1) */
        }

        result *= sum;
    }

    /* if a prime factor remains */
    if (t > 1) {
        result *= (1 + t);
    }

    return (seq_t)result;
}
