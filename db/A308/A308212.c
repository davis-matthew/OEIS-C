/*
 * A308212  Number of integer-sided triangles with perimeter n and sides a, b
 *          and c such that a <= b <= c and the length of side a is coprime to
 *          b.
 * https://oeis.org/A308212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer-sided triangles with perimeter n where
 *            the sides a, b, c satisfy a ≤ b ≤ c, a + b > c, and a and b are
 *            coprime
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    seq_t count = 0;
    idx_t a_max = n / 3;
    for (idx_t a = 1; a <= a_max; ++a) {
        idx_t b_min = a;
        /* Ensure a + b > n/2  ->  b > n/2 - a */
        idx_t boundary = (n / 2) - a + 1;
        if (boundary > b_min) b_min = boundary;
        idx_t b_max = (n - a) / 2;
        if (b_min > b_max) continue;
        for (idx_t b = b_min; b <= b_max; ++b) {
            if (gcd((seq_t)a, (seq_t)b) == (seq_t)1) {
                ++count;
            }
        }
    }
    return count;
}
