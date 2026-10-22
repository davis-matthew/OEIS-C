/*
 * A343270  Number of partitions of n into 3 parts x,y,z such that (x+y+z) |
 *          x*y*z.
 * https://oeis.org/A343270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts unordered triples (x,y,z) of positive integers *
 *            with x ≤ y ≤ z, x+y+z = n, such that n | x*y*z
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage: seq_t and idx_t are provided by the build system. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;

    seq_t count = 0;
    idx_t max_x = n / 3;

    for (idx_t x = 1; x <= max_x; ++x) {
        /* y must satisfy x ≤ y ≤ (n - x) / 2 to keep y ≤ z. */
        idx_t max_y = (n - x) / 2;
        for (idx_t y = x; y <= max_y; ++y) {
            idx_t z = n - x - y;          /* z ≥ y automatically */
            /* Compute (x*y*z) mod n using 64-bit intermediates. */
            unsigned long long prod = (unsigned long long)x * y;
            prod = prod * z;
            if (prod % n == 0) {
                count++;
            }
        }
    }

    return count;
}
