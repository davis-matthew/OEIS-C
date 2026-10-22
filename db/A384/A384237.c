/*
 * A384237  The number of divisors d of n such that d^d == d (mod n).
 * https://oeis.org/A384237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384237: a(n) = number of divisors d of n such that d^d ≡ d (mod n)  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;       /* offset begins at 1; n=0 not used */
    seq_t count = 1;            /* divisor n itself always satisfies */

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* first divisor d = i */
            idx_t d = i;
            idx_t m = n / d;
            if (d < n) {
                if (mod_pow((seq_t)d, (uint64_t)(d - 1), (seq_t)m) == 1)
                    ++count;
            }

            /* paired divisor d = j = n / i */
            idx_t j = n / i;
            if (j != i && j < n) {
                idx_t d2 = j;
                idx_t m2 = n / d2;   /* equals i */
                if (mod_pow((seq_t)d2, (uint64_t)(d2 - 1), (seq_t)m2) == 1)
                    ++count;
            }
        }
    }
    return count;
}
