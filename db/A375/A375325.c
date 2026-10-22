/*
 * A375325  a(n) is the number of divisors of n which are Loeschian numbers
 *          (A003136).
 * https://oeis.org/A375325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (p%3==2 ? ⌊e/2⌋+1 : e+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   loeschian_factor_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/loeschian_factor_value.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    if (m <= 1) return 1;          /* a(1) = 1 */

    seq_t result = 1;

    /* factor 2 */
    uint64_t exp = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        exp++;
    }
    if (exp > 0)
        result *= loeschian_factor_value(2, exp);

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            result *= loeschian_factor_value(p, exp);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1)
        result *= loeschian_factor_value(m, 1);

    return result;
}
