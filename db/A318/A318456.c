/*
 * A318456  a(n) = n OR A001065(n), where OR is bitwise-or (A003986) and
 *          A001065 = sum of proper divisors.
 * https://oeis.org/A318456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n OR A001065(n) (A001065 = sum of proper divisors)
 * Range:     u8 n <= 143, u16 n <= 25199, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 99 terms through n = 9767 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    seq_t num   = (seq_t)n;      /* n as seq_t */
    seq_t sigma = sigma_of(n);   /* sum of all divisors of n */
    seq_t proper = sigma - num;  /* sum of proper divisors */
    return (num | proper);       /* bitwise OR of n and its proper divisor sum */
}
