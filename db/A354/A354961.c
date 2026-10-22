/*
 * A354961  Coordination sequence for the Manhattan cubic lattice.
 * https://oeis.org/A354961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n^2 - 7*n + 7 for even n ≥ 6, a(n) = 4*n^2 - 9*n + 12
 *            for odd n ≥ 5, with a(0)=1, a(1)=3, a(2)=9, a(3)=21, a(4)=42
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_BASE 5
static const seq_t base_vals[MAX_BASE] = {
    (seq_t)1,  // a(0)
    (seq_t)3,  // a(1)
    (seq_t)9,  // a(2)
    (seq_t)21, // a(3)
    (seq_t)42  // a(4)
};

seq_t a(idx_t n) {
    if (n < MAX_BASE)
        return base_vals[n];
    seq_t nn = (seq_t)n;
    if ((n & 1ULL) == 0) {          // even n
        seq_t tmp = nn * nn;        // n^2
        return (seq_t)4 * tmp - (seq_t)7 * nn + (seq_t)7;
    } else {                         // odd n
        seq_t tmp = nn * nn;        // n^2
        return (seq_t)4 * tmp - (seq_t)9 * nn + (seq_t)12;
    }
}
