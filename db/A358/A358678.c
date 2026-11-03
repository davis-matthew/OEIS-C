/*
 * A358678  a(n) = 1 if n is odd and sigma(n) == 2 mod 4, otherwise 0.
 * https://oeis.org/A358678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and σ(n) == 2 (mod 4), otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 8776 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

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
    if (n == 0) return 0;            /* offset is 1; n = 0 is out of range */
    if ((n & 1U) == 0) return 0;     /* n must be odd */

    seq_t s = sigma_of(n);           /* σ(n) – sum of all positive divisors */
    return ((s & (seq_t)3) == (seq_t)2) ? (seq_t)1 : (seq_t)0;
}
