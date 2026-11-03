/*
 * A385148  a(n) = A001065(A346878(n)).
 * https://oeis.org/A385148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ( σ(2*n) – 2*n ) – ( σ(2*n) – 2*n ) where σ(m) is the
 *            sum of all divisors of m
 * Range:     u8 n <= 44, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute the sum of divisors of x using integer arithmetic only. */
static seq_t sigma_div(seq_t x)
{
    if (x == 0)
        return 0;               /* convention for x = 0 */
    seq_t sum = 0;
    /* iterate d from 1 to floor(sqrt(x)) */
    for (idx_t d = 1; d <= x / d; ++d)
    {
        if (x % (seq_t)d == 0)
        {
            sum += (seq_t)d;
            seq_t d2 = x / (seq_t)d;
            if (d2 != (seq_t)d)
                sum += d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first element. */
    if (n == 0)
        return 0;

    /* compute m = σ(2*n) – 2*n */
    seq_t two_n = (seq_t)n << 1;            /* 2 * n */
    seq_t sigma_2n = sigma_div(two_n);
    seq_t m = sigma_2n - two_n;

    /* a(n) = σ(m) – m */
    seq_t sigma_m = sigma_div(m);
    return sigma_m - m;
}
