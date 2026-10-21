/*
 * A292688  Antidiagonals of the Sierpinski carpet (as binary numbers).
 * https://oeis.org/A292688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   cell_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = decimal integer formed by concatenating the binary
 * bits on the anti‑diagonal (i + j = n + 1) of the Sierpinski carpet
 * pattern A153490.
 */
#include <stdint.h>
#include "../utils/cell_value.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t j = n + 1 - i;
        seq_t bit = (seq_t)cell_value(i, j);
        res = res * 10 + bit;
    }
    return res;
}
