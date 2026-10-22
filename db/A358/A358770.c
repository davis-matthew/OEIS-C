/*
 * A358770  a(n) = 1 if n is of the form p * m^2, where p is an odd prime and
 *          m is a natural number >= 1, otherwise 0.
 * https://oeis.org/A358770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n can be written as p * m^2 where p is an odd prime
 *            and m is a positive integer; otherwise 0. This implementation
 *            determines whether the prime factorisation of n contains exactly
 *            one odd prime with an odd exponent and all other primes
 *            (including 2) have even exponents. It uses simple...
 * Range:     u8 n <= 9682, u16 n <= 9682, u32 n <= 9682, u64 n <= 9682,
 *            u128 n <= 9682, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 0 or 1 never has the required form */
    if (n < 2)
        return 0;

    uint64_t x = (uint64_t)n;    /* work in 64‑bit arithmetic */
    uint64_t e2 = 0;             /* exponent of 2 in n */

    /* factor out all powers of 2 */
    while ((x & 1ULL) == 0) {
        x >>= 1;
        e2++;
    }

    /* if the exponent of 2 is odd, n cannot be p*m^2 */
    if (e2 & 1ULL)
        return 0;

    uint64_t oddPrimeCnt = 0;    /* number of odd primes with odd exponent */

    /* factor the remaining odd part of x */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            if (exp & 1ULL) {            /* odd exponent */
                oddPrimeCnt++;
                if (oddPrimeCnt > 1)      /* more than one odd exponent */
                    return 0;
            }
        }
    }

    /* if x > 1, it is a remaining prime factor with exponent 1 */
    if (x > 1) {
        oddPrimeCnt++;
        if (oddPrimeCnt > 1)
            return 0;
    }

    /* exactly one odd prime with odd exponent → valid */
    return (seq_t)(oddPrimeCnt == 1);
}
