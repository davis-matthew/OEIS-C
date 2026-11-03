/*
 * A399723  a(n) is the greatest k <= n such that k divides sigma(n), where
 *          sigma = A000203.
 * https://oeis.org/A399723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = max{k ≤ n : k divides σ(n)}, where σ is the divisor sum
 *            function
 * Range:     u8 n <= 286, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 8119 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    if (n == 0) return 0;  /* undefined for n < 1, but graceful */

    seq_t sigma = sigma_of(n);

    for (idx_t k = n; k > 0; --k) {
        seq_t kseq = (seq_t)k;
        if (sigma % kseq == 0)
            return kseq;
    }

    return (seq_t)1;  /* should never be reached for n ≥ 1 */
}
