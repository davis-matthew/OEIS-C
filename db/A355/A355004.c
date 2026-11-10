/*
 * A355004  a(n) = Sum_{k=0..n} A271703(k + n, n), row sums of A355005.
 * https://oeis.org/A355004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A271703(k+n, n), where A271703(m, n) =
 *            binomial(m-1, n-1)*m!/n! for n > 0 and a(0) = 1. Each term is
 *            C(n+k-1, k) * prod_{j=1..k} (n+j)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t c = 1;   /* C(n+k-1, k) */
    seq_t P = 1;   /* prod_{j=1..k} (n+j) */
    seq_t sum = 1; /* k = 0 term */
    for (idx_t k = 1; k <= n; ++k) {
        c = c * (seq_t)(n + k - 1) / (seq_t)k;
        P = P * (seq_t)(n + k);
        sum += c * P;
    }
    return sum;
}
