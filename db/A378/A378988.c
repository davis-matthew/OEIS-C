/*
 * A378988  a(n) = 2*n XOR 1+sigma(n), where XOR is bitwise-xor, A003987.
 * https://oeis.org/A378988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (2*n) XOR (1 + sigma(n))
 * Range:     u8 n <= 107, u16 n <= 17639, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 95 terms through n = 9917 (2026-11-01)
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
    seq_t two_n  = (seq_t)n * 2;
    seq_t sigma1 = sigma_of(n) + 1;
    return two_n ^ sigma1;
}
