/*
 * A344329  Number of divisors of n^6.
 * https://oeis.org/A344329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (6*e + 1) (number of divisors of n⁶)
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n = 0, return 0 for safety */

    idx_t temp = n;
    seq_t result = 1;

    /* factor 2 separately */
    idx_t exp = 0;
    while ((temp & 1ULL) == 0) {
        exp++;
        temp >>= 1;
    }
    if (exp > 0) {
        result *= (seq_t)(6 * exp + 1);
    }

    /* factor odd primes */
    for (idx_t i = 3; i <= temp / i; i += 2) {
        if (temp % i == 0) {
            exp = 0;
            while (temp % i == 0) {
                exp++;
                temp /= i;
            }
            result *= (seq_t)(6 * exp + 1);
        }
    }

    /* if a prime factor remains */
    if (temp > 1) {
        result *= 7; /* exp = 1 ⇒ 6*1+1 = 7 */
    }

    return result;
}
