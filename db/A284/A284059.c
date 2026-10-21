/*
 * A284059  The absolute values of A275966.
 * https://oeis.org/A284059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = |Re(I*Sum_{d|n}(mu(d)*I^(n/d)))|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _value_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the multiplicative property: */
/* a(p^e) = |Re(I^(p^e+1) - I^(p^(e-1)+1))| */

/* The implementation below is purely integer arithmetic and works
   for any seq_t and idx_t types.  It is O(√n) per call, easily
   under 2 s for n up to 10 000. */
#include <stdint.h>
#include "../utils/_value_prime_power.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;          /* undefined offset, return 0 for safety */

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* trial division up to sqrt(n) */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= _value_prime_power(p, exp);
        }
    }
    if (temp > 1) { /* remaining prime factor */
        result *= _value_prime_power(temp, 1);
    }

    return result;
}
