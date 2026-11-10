/*
 * A332264  Partial sums of A334136.
 * https://oeis.org/A332264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (k-1)*sigma(k) = Sum_{d=1..n} d*(d*T(m) -
 *            m), where m = floor(n/d) and T(m) = m*(m+1)/2
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t total = 0;
    for (idx_t d = 1; d <= n; ++d) {
        seq_t m = (seq_t)(n / d);
        seq_t T = m * (m + 1) / 2;
        seq_t D = (seq_t)d;
        total += D * (D * T - m);
    }
    return total;
}
