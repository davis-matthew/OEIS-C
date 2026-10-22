/*
 * A327274  Dirichlet g.f.: 1 / (zeta(s)^2 * (1 - 2^(1 - s))).
 * https://oeis.org/A327274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   Multiplicative definition: a(1) = 1 a(2) = 0 a(2^e) = 2^(e-2)
 *            for e ≥ 2 a(p) = -2 for odd prime p a(p^2) = 1 a(p^e) = 0 for
 *            odd prime p, e ≥ 3 In particular, a(n) = 0 if n ≡ 2 (mod 4)
 * Range:     i8 n <= 511, i16 n <= 8878, i32 n <= 8878, i64 n <= 8878,
 *            i128 n <= 8878, big any n
 * Helpers:   _smallest_prime_factor, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    // Factor out powers of two
    idx_t e2 = 0;
    idx_t temp = n;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e2;
    }

    if (e2 == 1) return (seq_t)0;          // a(2) or any n with a single 2
    seq_t result = 1;
    if (e2 >= 2) {
        result = big_pow((seq_t)2, (seq_t)(e2 - 2));  // 2^(e2-2)
    }

    if (temp == 1) return result;          // n was a power of two

    // Factor the odd part
    while (temp > 1) {
        uint64_t sp = _smallest_prime_factor(temp);
        idx_t exp = 0;
        do {
            temp /= sp;
            ++exp;
        } while (temp % sp == 0);

        if (exp >= 3) {                    // any odd prime power ≥3 yields 0
            return (seq_t)0;
        } else if (exp == 1) {
            result *= (seq_t)-2;
        } /* for exp == 2 we multiply by 1, no change */
    }
    return result;
}
