/*
 * A374915  a(n) = (n - 1) * (n - 2) * sigma(n).
 * https://oeis.org/A374915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(n-2)*sigma(n) where sigma(n) = sum of divisors of
 *            n
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
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
    /* n is 1-based index. For n=1 or 2 the multiplicative factor vanishes. */
    if (n < 3)
        return 0;

    seq_t s = sigma_of(n);                /* sigma(n) */
    seq_t term = ((seq_t)n - 1) * ((seq_t)n - 2) * s;
    return term;
}
