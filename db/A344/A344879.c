/*
 * A344879  a(n) = A344875(n) / A344878(n), where A344875(n) is multiplicative
 *          with a(2^e) = 2^(1+e) - 1, and a(p^e) = p^e -1 for odd primes p,
 *          and A344878(n) gives the least common multiple of the same
 *          factors.
 * https://oeis.org/A344879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^e||n} f(p^e) / lcm_{p^e||n} f(p^e), where
 *            f(p^e)=p^e-1 for odd primes p and f(2^e)=2^(e+1)-1
 * Range:     u8 n <= 1101, u16 n <= 9852, u32 n <= 9852, u64 n <= 9852,
 *            u128 n <= 9852, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;   /* undefined for n < 1, but harmless */

    uint64_t temp = (uint64_t)n;
    seq_t prod = 1;
    seq_t lcm  = 1;

    uint64_t d = 2;
    while (d * d <= temp) {
        if (temp % d == 0) {
            uint64_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                e++;
            }
            seq_t factor;
            if (d == 2) {
                /* 2^(e+1) - 1 */
                factor = ((seq_t)1 << (e + 1)) - 1;
            } else {
                /* d^e - 1 */
                seq_t power = 1;
                for (uint64_t i = 0; i < e; ++i)
                    power *= (seq_t)d;
                factor = power - 1;
            }
            prod *= factor;
            seq_t g = gcd(lcm, factor);
            lcm = lcm / g * factor;
        }
        d++;
    }

    if (temp > 1) { /* remaining prime factor with exponent 1 */
        uint64_t d_prime = temp;
        seq_t factor;
        if (d_prime == 2) {
            factor = 3; /* 2^(1+1)-1 = 3 */
        } else {
            factor = (seq_t)d_prime - 1; /* d_prime^1 - 1 */
        }
        prod *= factor;
        seq_t g = gcd(lcm, factor);
        lcm = lcm / g * factor;
    }

    return prod / lcm;
}
