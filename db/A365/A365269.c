/*
 * A365269  a(n) = Product_{k=1..n} A002720(k).
 * https://oeis.org/A365269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} A002720(k), with A002720(k) =
 *            2*k*A002720(k-1) - (k-1)^2*A002720(k-2), A002720(0) = 1,
 *            A002720(1) = 2; a(0) = 1 (empty product)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prod = 1;
    seq_t prev2 = 1;
    seq_t prev1 = 2;
    if (n == 0) return prod;
    prod = 2;
    for (idx_t i = 2; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t sm = si - 1;
        seq_t curr = 2 * si * prev1 - sm * sm * prev2;
        prev2 = prev1;
        prev1 = curr;
        prod = prod * curr;
    }
    return prod;
}
