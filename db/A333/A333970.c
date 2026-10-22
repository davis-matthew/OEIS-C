/*
 * A333970  Irregular triangle read by rows where the n-th row lists the bases
 *          2<=b<=n+1 where n in base b contains the digit b-1.
 * https://oeis.org/A333970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf, uned
 * Formula:   a(n) concatenates, row by row, the bases b (2 ≤ b ≤ row+1) for
 *            which n written in base b contains the digit b‑1. The sequence
 *            is flattened row-wise
 * Range:     u8 n <= 1896, u16 n <= 9684, u32 n <= 9684, u64 n <= 9684,
 *            u128 n <= 9684, big any n
 * Helpers:   contains_digit_b_minus_1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/contains_digit_b_minus_1.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t pos = 1;          /* current position in the flattened sequence */
    for (idx_t row = 1;; ++row) {
        for (idx_t b = 2; b <= row + 1; ++b) {
            if (contains_digit_b_minus_1(row, b)) {
                if (pos == n)
                    return (seq_t)b;
                ++pos;
            }
        }
    }
}
