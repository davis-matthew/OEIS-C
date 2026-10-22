/*
 * A394038  a(n) = number of triples (x, y, z) such that 2*x^2 + y*z = n,
 *          where x, y, z are positive integers satisfying 0 < x < y < z.
 * https://oeis.org/A394038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: brute‑force count of triples (x,y,z) where
   0 < x < y < z and 2*x^2 + y*z = n */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t x = 1; ; ++x) {
        seq_t s2x = (seq_t)2 * (seq_t)x * (seq_t)x;
        if (s2x >= (seq_t)n) break;
        seq_t T = (seq_t)n - s2x;
        idx_t sqrtT = (idx_t)floor_sqrt(T);
        if ((idx_t)(x + 1) > sqrtT) continue;
        for (idx_t y = x + 1; y <= sqrtT; ++y) {
            if (T % (seq_t)y == 0) {
                seq_t z = T / (seq_t)y;
                if (z > (seq_t)y) count++;
            }
        }
    }
    return count;
}
