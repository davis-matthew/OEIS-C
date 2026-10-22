/*
 * A373898  Number of divisors d of n such that (n/d)^d == d (mod (n/d + d)).
 * https://oeis.org/A373898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { d | n : (n/d)^d ≡ d (mod (n/d + d)) }
 * Range:     u8 n <= 7279, u16 n <= 9342, u32 n <= 9342, u64 n <= 9342,
 *            u128 n <= 9342, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* first divisor */
            idx_t d1 = i;
            idx_t t1 = n / d1;
            seq_t mod1 = (seq_t)(t1 + d1);
            seq_t pow1 = mod_pow((seq_t)t1, d1, mod1);
            if (pow1 == (seq_t)d1) ++count;

            /* paired divisor */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t t2 = n / d2;
                seq_t mod2 = (seq_t)(t2 + d2);
                seq_t pow2 = mod_pow((seq_t)t2, d2, mod2);
                if (pow2 == (seq_t)d2) ++count;
            }
        }
    }

    return count;
}
