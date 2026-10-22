/*
 * A365210  The number of divisors d of n such that gcd(d, n/d) is a 5-rough
 *          number (A007310).
 * https://oeis.org/A365210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} (p==2||p==3 ? 2 : e+1), with a(1)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* offset 1, but guard against 0 if called */
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    while (m > 1) {
        idx_t p;         /* smallest prime divisor of m */

        /* handle 2 and 3 directly */
        if (m <= 3) {
            p = m;       /* m is 2 or 3 */
        } else if ((m & 1) == 0) {
            p = 2;
        } else if ((m % 3) == 0) {
            p = 3;
        } else {
            /* trial division by 6k±1 primes: 5,7,11,13,... */
            bool found = false;
            for (idx_t i = 5; i <= m / i; i += 6) {
                if (m % i == 0) {
                    p = i;
                    found = true;
                    break;
                }
                if (m % (i + 2) == 0) {
                    p = i + 2;
                    found = true;
                    break;
                }
            }
            if (!found)           /* m is prime */
                p = m;
        }

        /* count exponent e of p */
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }

        /* multiply the contribution of this prime power */
        seq_t contrib = (p == 2 || p == 3) ? (seq_t)2 : (seq_t)(e + 1);
        result *= contrib;
    }

    return result;
}
