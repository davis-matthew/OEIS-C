/*
 * A292689  Decimal values of the antidiagonals of the Sierpinski carpet
 *          considered as binary numbers.
 * https://oeis.org/A292689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal value of the binary number formed by the (n‑1)-st
 *            antidiagonal of the Sierpinski carpet. For each i in [0, n‑1],
 *            let j = n‑1‑i. The bit is 1 iff in the base‑3 representations of
 *            i and j no digit position has digit 1 in both numbers. The bits
 *            are concatenated with i increasing from...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   ssc_cell_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/ssc_cell_bit.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t k = n - 1;
    for (idx_t i = 0; i < n; ++i) {
        idx_t j = k - i;
        int bit = ssc_cell_bit(i, j);
        res = res * 2 + (seq_t)bit;
    }
    return res;
}
