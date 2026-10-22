/*
 * A375576  a(n) is the number of partitions n = x + y + z of positive
 *          integers such that x*y*z is a perfect square.
 * https://oeis.org/A375576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions n = x + y + z (x ≤ y ≤ z,
 *            x, y, z > 0) such that x*y*z is a perfect square
 * Range:     u8 n <= 11, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n < 3) return 0;

    for (idx_t x = 1; x <= n - 2; ++x) {
        for (idx_t y = x; y <= n - x - 1; ++y) {
            idx_t z = n - x - y;
            if (z < y) break;                 // keep nondecreasing order
            seq_t prod = (seq_t)x * (seq_t)y * (seq_t)z;
            seq_t sq   = floor_sqrt(prod);
            if (sq * sq == prod) ++count;     // product is a perfect square
        }
    }
    return count;
}
