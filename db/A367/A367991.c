/*
 * A367991  The sum of the divisors of the squarefree part of n.
 * https://oeis.org/A367991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p | n, exp_p(n) odd} (1 + p)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined in OEIS, guard against misuse */

    idx_t t = n;
    seq_t res = 1;

    /* factor n by trial division; collect primes with odd exponent */
    for (idx_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            /* determine if exponent of p is odd */
            int odd = 0;
            do {
                t /= p;
                odd = !odd;
            } while (t % p == 0);
            if (odd) {
                res *= (seq_t)(1 + p);
            }
        }
    }

    /* if a prime factor larger than sqrt(n) remains (exponent 1) */
    if (t > 1) {
        res *= (seq_t)(1 + t);
    }

    return res;
}
