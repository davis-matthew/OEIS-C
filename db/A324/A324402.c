/*
 * A324402  a(n) = Product_{i=1..n, j=1..n} (2*i + j).
 * https://oeis.org/A324402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n, j=1..n} (2*i + j)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1, a(1) = 3, ... (see OEIS entry A324402) */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = 1; j <= n; ++j) {
            seq_t term = (seq_t)(2 * i + j);
            result *= term;
        }
    }
    return result;
}
