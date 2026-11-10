/*
 * A339576  Row sums of triangle A236104.
 * https://oeis.org/A339576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..m} T(n,k)^2, m = floor((sqrt(8n+1)-1)/2),
 *            T(n,k) = ceil((n+1)/k - (k+1)/2) (row n of A235791)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return 0;
    for (idx_t k = 1; k * (k + 1) / 2 <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t num = (seq_t)2 * ((seq_t)n + 1) - kk * (kk + 1);
        seq_t den = (seq_t)2 * kk;
        seq_t v = (num + den - 1) / den;
        sum += v * v;
    }
    return sum;
}
