/*
 * A241657  The sum of a^2 + b^2 for all nonnegative integers a,b such that
 *          b^2 - a^2 = 2*n+1.
 * https://oeis.org/A241657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = ((∑_{d | (2*n+1)} d²) + m) / 2, * where m = 2*n+1 if m
 *            is a perfect square, otherwise m = 0
 * Range:     u8 n <= 6, u16 n <= 170, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, not strictly required but harmless */

/* Simple integer square‑root for 64‑bit values – safe for n ≤ 10000. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    /* Increase r until (r+1)² would exceed x. */
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    /* m is odd: 2*n + 1 */
    idx_t m = 2 * n + 1;

    seq_t sumsq = 0;    /* Σ d² over all divisors d of m */

    /* Enumerate divisors up to √m */
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            seq_t dsq = (seq_t)d * (seq_t)d;
            sumsq += dsq;           /* contribution of divisor d */

            idx_t q = m / d;        /* paired divisor q = m / d */
            if (q != d) {
                seq_t qsq = (seq_t)q * (seq_t)q;
                sumsq += qsq;       /* contribution of divisor q */
            }
        }
    }

    /* Add m once if m is a perfect square. */
    seq_t add = 0;
    idx_t sqrt_m = isqrt_u64(m);
    if (sqrt_m * sqrt_m == m) {
        add = (seq_t)m;
    }

    return (sumsq + add) / 2;
}
