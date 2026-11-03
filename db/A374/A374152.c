/*
 * A374152  Number of divisors d of n such that d^(n/d) == d (mod (d + n/d)).
 * https://oeis.org/A374152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of divisors d of n such that d^(n/d) ≡ d (mod (d +
 *            n/d))
 * Range:     u8 n <= 3315, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9693 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d1 = 1; d1 <= n / d1; ++d1) {
        if (n % d1 == 0) {
            /* divisor d1 */
            idx_t m1 = n / d1;
            seq_t mod1 = (seq_t)d1 + (seq_t)m1;
            seq_t pow1 = powmod((seq_t)d1, (uint64_t)m1, mod1);
            if (pow1 == (seq_t)d1) ++count;

            /* paired divisor d2, if distinct */
            idx_t d2 = m1;
            if (d2 != d1) {
                idx_t m2 = n / d2;   /* m2 == d1 */
                seq_t mod2 = (seq_t)d2 + (seq_t)m2;
                seq_t pow2 = powmod((seq_t)d2, (uint64_t)m2, mod2);
                if (pow2 == (seq_t)d2) ++count;
            }
        }
    }
    return count;
}
