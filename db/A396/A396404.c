/*
 * A396404  Antidiagonal sums of A396403.
 * https://oeis.org/A396404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} j^(i*(i-1)/2) with i = n-j, taking 0^0 = 1
 *            (antidiagonal sums of A(i,j) = j^(i*(i-1)/2))
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-10)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 0; j <= n; j++) {
        idx_t i = n - j;
        idx_t e = (i == 0) ? 0 : (i % 2 == 0 ? (i / 2) * (i - 1) : i * ((i - 1) / 2));
        if (j == 0) {
            if (e == 0) sum += 1;
        } else if (j == 1) {
            sum += 1;
        } else {
            sum += ipow_u64_idx((uint64_t)j, e);
        }
    }
    return sum;
}
