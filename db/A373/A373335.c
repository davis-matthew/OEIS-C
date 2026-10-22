/*
 * A373335  Expansion of Sum_{k>=1} x^k / (1 + x^k + x^(2*k) + x^(3*k) +
 *          x^(4*k)).
 * https://oeis.org/A373335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A373335:
   a(n) = Σ_{d|n} ( [d ≡ 1 (mod 5)] - [d ≡ 2 (mod 5)] )   */
/* This runs in O(√n) by enumerating all divisors of n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not part of the OEIS offset */

    seq_t result = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) continue;

        /* divisor d itself */
        idx_t m = d % 5;
        if (m == 1) ++result;
        else if (m == 2) --result;

        /* paired divisor n/d, if distinct */
        idx_t other = n / d;
        if (other != d) {
            m = other % 5;
            if (m == 1) ++result;
            else if (m == 2) --result;
        }
    }
    return result;
}
