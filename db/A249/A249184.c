/*
 * A249184  A249183 seen as binary numbers.
 * https://oeis.org/A249184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..2n} A249133(n,k)*2^k, where row n of A249133
 *            has entries: k=2n -> 1; k even -> C(n,k/2) mod 2; k odd ->
 *            C(n-1,(k-1)/2) mod 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 979 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int A249183_bit(idx_t r, idx_t pos)
{
    if (pos == 2 * r) return 1;
    idx_t i = pos >> 1;
    idx_t m = (pos & 1) ? (r - 1) : r;
    return ((i & (m - i)) == 0) ? 1 : 0;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* Horner from the top bit (k = 2n) down to k = 0 */
    for (idx_t j = 0; j <= 2 * n; j++) {
        idx_t k = 2 * n - j;
        res = res * 2 + (seq_t)A249183_bit(n, k);
    }
    return res;
}
