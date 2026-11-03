/*
 * A336923  a(n) = 1 if sigma(2n) - sigma(n) is a power of 2, otherwise 0.
 * https://oeis.org/A336923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 1 if sigma(2n) - sigma(n) is a power of 2, else 0
 * Range:     u8 n <= 83, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 7432 (2026-11-01)
 * Helpers:   is_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_pow2.h"

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
    seq_t sigma_n  = sigma_of(n);
    seq_t sigma_2n = sigma_of(n * 2);
    seq_t diff     = sigma_2n - sigma_n;
    return is_pow2(diff) ? (seq_t)1 : (seq_t)0;
}
