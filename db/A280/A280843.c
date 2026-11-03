/*
 * A280843  a(n) = A049502(sigma(n)).
 * https://oeis.org/A280843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A049502(A000203(n))
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 125 terms through n = 9073 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A049502(n) = sum of positions (starting at 1 from the least‑significant bit)
   where a bit is 1 and the next higher bit is 0. */
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
    seq_t sigma = sigma_of(n);          /* sigma(n) */
    seq_t major = 0;
    idx_t  pos   = 1;
    seq_t  x     = sigma;

    while (x != 0) {
        if ((x & 1) && !((x >> 1) & 1) && ((x >> 1) != 0))
            major += (seq_t)pos;
        x >>= 1;
        pos++;
    }
    return major;
}
