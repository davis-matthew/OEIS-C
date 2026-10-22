/*
 * A370784  a(n) is the denominator of the sum of the reciprocals of the
 *          squarefree divisors of the powerful part of n.
 * https://oeis.org/A370784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of the sum of reciprocals of the * squarefree
 *            divisors of the powerful part of n. * For n = ∏ p^e, let P be
 *            the set of primes with e≥2. * Then a(n) = (∏_{p∈P} p) /
 *            gcd(∏_{p∈P} (p+1), ∏_{p∈P} p)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    idx_t temp = n;
    bool has_power = false;
    seq_t rad = 1;          /* product of primes with exponent ≥ 2 */
    seq_t prod = 1;         /* product of (p+1) for such primes */

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (exp >= 2) {
                has_power = true;
                rad *= (seq_t)p;
                prod *= (seq_t)(p + 1);
            }
        }
    }

    /* remaining prime factor */
    if (temp > 1) {
        /* exponent is 1, so it does not contribute to powerful part */
        /* nothing to do */
    }

    if (!has_power) {
        /* n is squarefree, powerful part is 1, denominator is 1 */
        return 1;
    }

    seq_t g = gcd(prod, rad);
    return rad / g;
}
