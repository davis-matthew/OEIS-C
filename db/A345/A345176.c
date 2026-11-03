/*
 * A345176  a(n) = Sum_{k=1..n} floor(n/k)^k.
 * https://oeis.org/A345176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k)^k
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 59, u64 n <= 119,
 *            u128 n <= 241, big any n
 * Validated: standard, pass, 58 terms through n = 4857 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ) {
        idx_t q = n / k;
        idx_t next = n / q + 1;
        idx_t k_end = next - 1;
        idx_t len = k_end - k + 1;

        if (q == 1) {
            total += (seq_t)len;
        } else {
            seq_t u = ipow_i32((seq_t)q, (int)k);
            seq_t powlen = ipow_i32((seq_t)q, (int)len);
            seq_t term = u * (powlen - 1);
            seq_t denom = (seq_t)(q - 1);
            term = term / denom;
            total += term;
        }
        k = next;
    }
    return total;
}
