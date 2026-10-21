/*
 * A293484  The number of 7th powers in the multiplicative group modulo n.
 * https://oeis.org/A293484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = φ(n) / gcd(φ(n),7) where φ is Euler's totient function.
 *            The function is multiplicative; for a prime power p^e: φ(p^e) =
 *            (p−1)·p^(e−1) a(p^e) = φ(p^e) / (7 if φ(p^e) is divisible by 7
 *            else 1)
 * Range:     u8 n <= 256, u16 n <= 9549, u32 n <= 9549, u64 n <= 9549,
 *            u128 n <= 9549, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"   /* provides seq_t pow_uint(uint64_t, idx_t) */

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n=0, return 0 for safety */
        return 0;

    seq_t result = 1;
    idx_t tmp = n;

    /* factor out powers of 2 */
    if (tmp % 2 == 0) {
        idx_t e = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++e;
        }
        seq_t phi = pow_uint(2, e - 1);          /* (2-1)=1 so phi = 2^(e-1) */
        if (phi % 7 == 0)
            phi = phi / 7;                       /* never happens for 2, but harmless */
        result *= phi;
    }

    /* trial division with odd primes */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t phi = (seq_t)(p - 1) * pow_uint(p, e - 1);
            if (phi % 7 == 0)
                phi /= 7;
            result *= phi;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (tmp > 1) {
        idx_t p = tmp;
        seq_t phi = (seq_t)(p - 1);              /* e = 1, so pow_uint(p,0)=1 */
        if (phi % 7 == 0)
            phi /= 7;
        result *= phi;
    }

    return result;
}
