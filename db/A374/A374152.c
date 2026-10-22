/*
 * A374152  Number of divisors d of n such that d^(n/d) == d (mod (d + n/d)).
 * https://oeis.org/A374152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of divisors d of n such that d^(n/d) ≡ d (mod (d +
 *            n/d))
 * Range:     u8 n <= 3315, u16 n <= 9693, u32 n <= 9693, u64 n <= 9693,
 *            u128 n <= 9693, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d1 = 1; d1 <= n / d1; ++d1) {
        if (n % d1 == 0) {
            /* divisor d1 */
            idx_t m1 = n / d1;
            seq_t mod1 = (seq_t)d1 + (seq_t)m1;
            seq_t pow1 = mod_pow((seq_t)d1, (uint64_t)m1, mod1);
            if (pow1 == (seq_t)d1) ++count;

            /* paired divisor d2, if distinct */
            idx_t d2 = m1;
            if (d2 != d1) {
                idx_t m2 = n / d2;   /* m2 == d1 */
                seq_t mod2 = (seq_t)d2 + (seq_t)m2;
                seq_t pow2 = mod_pow((seq_t)d2, (uint64_t)m2, mod2);
                if (pow2 == (seq_t)d2) ++count;
            }
        }
    }
    return count;
}
