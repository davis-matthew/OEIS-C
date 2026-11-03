/*
 * A259369  a(n) = 1 + sigma(n)^3 + sigma(n)^6.
 * https://oeis.org/A259369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sigma(n)^3 + sigma(n)^6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 19, u64 n <= 539,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 42 terms through n = 9650 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t s  = sigma_of(n);              /* sum of divisors of n */
    seq_t c3 = ipow_idx(s, (idx_t)3);     /* s^3   */
    seq_t c6 = ipow_idx(s, (idx_t)6);     /* s^6   */
    return ((seq_t)1) + c3 + c6;         /* 1 + s^3 + s^6 */
}
