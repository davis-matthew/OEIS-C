/*
 * A318466  a(n) = 2*n OR A000203(n), where OR is bitwise-or (A003986) and
 *          A000203 = sum of divisors.
 * https://oeis.org/A318466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = 2*n OR sigma(n)
 * Range:     u8 n <= 107, u16 n <= 17639, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 87 terms through n = 9465 (2026-11-01)
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
    seq_t s = sigma_of(n);       /* sum of divisors of n */
    seq_t base = (seq_t)n << 1;  /* 2*n as seq_t */
    return base | s;             /* bitwise OR with sigma(n) */
}
