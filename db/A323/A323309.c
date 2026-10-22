/*
 * A323309  The sum of exponential semiproper divisors of n.
 * https://oeis.org/A323309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e | n of * a(p^e) where
 *            a(p^e) = p for e = 1 and a(p^e) = p^e + p otherwise
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   part_for_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/part_for_prime_power.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined, but harmless */
    if (n == 1) return 1;       /* a(1)=1 by definition */

    seq_t result = 1;
    idx_t temp = n;

    /* factor the power of two */
    idx_t exp = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++exp;
    }
    if (exp > 0) {
        seq_t p = 2;
        result *= part_for_prime_power(p, exp);
    }

    /* factor odd primes by trial division */
    for (idx_t d = 3; d * d <= temp; d += 2) {
        if (temp % d == 0) {
            idx_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }
            seq_t p = d;
            result *= part_for_prime_power(p, e);
        }
    }

    /* if a prime factor remains (>1) it has exponent 1 */
    if (temp > 1) {
        seq_t p = temp;
        result *= p;
    }

    return result;
}
