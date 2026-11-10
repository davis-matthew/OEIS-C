/*
 * A338723  Alternating row sums in triangle A338721.
 * https://oeis.org/A338723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..K} (-1)^(k+1) * (2*floor((n - k*(k-1)/2)/k) -
 *            1), where K = max k with k*(k+1)/2 <= n (T(n,k) of A338721
 *            summed with alternating signs along row n)
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9942 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t k = 1;
    while (k * (k + 1) / 2 <= n) {
        idx_t m = (n - k * (k - 1) / 2) / k;
        seq_t v = (seq_t)m * 2 - 1;
        if (k & 1) pos += v; else neg += v;
        ++k;
    }
    return pos - neg;
}
