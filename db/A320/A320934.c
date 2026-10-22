/*
 * A320934  Number of chiral pairs of color patterns (set partitions) for a
 *          row of length n using 4 or fewer colors (subsets).
 * https://oeis.org/A320934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n - 4^(floor(n/2)+1))/48
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* Compute 4^n using the auxiliary power routine */
    seq_t pow_n = pow_uint(4, n);

    /* m = floor(n/2) + 1  (integer division truncates) */
    seq_t pow_m = pow_uint(4, n / 2 + 1);

    /* Difference and division by 48 */
    seq_t diff = pow_n - pow_m;
    diff /= 48;
    return diff;
}
