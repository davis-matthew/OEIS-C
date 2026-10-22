/*
 * A366078  The number of distinct prime factors of the cubefree part of n
 *          (A360539).
 * https://oeis.org/A366078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes with exponent ≤ 2 in the prime
 *            factorization of n (i.e. the number of distinct prime factors of
 *            the cubefree part of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;          /* work with a native type */
    seq_t  cnt = 0;

    if (nn == 0) return 0;              /* undefined, but return 0 for safety */

    /* factor 2 separately */
    if ((nn & 1ULL) == 0) {
        uint64_t exp = 0;
        while ((nn & 1ULL) == 0) {
            nn >>= 1;
            exp++;
        }
        if (exp <= 2) cnt++;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= nn / p; p += 2) {
        if (nn % p == 0) {
            uint64_t exp = 0;
            while (nn % p == 0) {
                nn /= p;
                exp++;
            }
            if (exp <= 2) cnt++;
        }
    }

    /* remaining prime factor, if any */
    if (nn > 1) {
        /* exponent is 1, which is ≤ 2 */
        cnt++;
    }

    return cnt;
}
