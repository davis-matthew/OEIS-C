/*
 * A378550  a(n) = 1 if sigma(n) >= 2*n-1, otherwise 0.
 * https://oeis.org/A378550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [sigma(n) >= 2*n-1]
 * Range:     u8 n <= 107, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 144 terms through n = 9731 (2026-11-01)
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

/* Return 1 if the sum of divisors of n is at least 2*n-1, otherwise 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; guard against n==0 */

    seq_t sig  = sigma_of(n);             /* sum of divisors of n */
    seq_t rhs  = ((seq_t)n << 1) - 1;     /* 2*n - 1 */

    return (sig >= rhs) ? (seq_t)1 : (seq_t)0;
}
