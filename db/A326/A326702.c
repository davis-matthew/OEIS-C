/*
 * A326702  Number of distinct vertices in the set-system with BII-number n.
 * https://oeis.org/A326702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): number of distinct vertices in the set-system with BII-number n.
   For each 1-bit at position i in the reversed binary expansion of n
   (positions start at 1), take the set of positions of 1-bits in the
   binary expansion of i. The union of all these sets gives the vertices,
   and a(n) is its cardinality. */
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t union_mask = 0;
    idx_t temp = n;
    idx_t pos = 1;
    while (temp) {
        if (temp & 1ULL) {
            idx_t p = pos;
            idx_t inner = p;
            idx_t inner_pos = 1;
            while (inner) {
                if (inner & 1ULL) {
                    union_mask |= ((idx_t)1 << (inner_pos - 1));
                }
                inner >>= 1;
                inner_pos++;
            }
        }
        temp >>= 1;
        pos++;
    }
    seq_t count = 0;
    idx_t mask = union_mask;
    while (mask) {
        count++;
        mask &= mask - 1;
    }
    return count;
}
