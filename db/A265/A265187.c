/*
 * A265187  Nonnegative m for which 2*floor(m^2/11) = floor(2*m^2/11).
 * https://oeis.org/A265187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/9)*11 + M[(n-1)%9] where M =
 *            {0,1,2,4,5,6,7,9,10} This lists non‑negative integers m that are
 *            not congruent to 3 or 8 modulo 11
 * Range:     u8 n <= 210, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* n is 1‑based index */
    idx_t blocks = (n - 1) / 9;          /* number of complete 9‑element blocks */
    idx_t r      = (n - 1) % 9;          /* position within the block */
    static const unsigned char rem_map[9] = {0, 1, 2, 4, 5, 6, 7, 9, 10};
    seq_t base = (seq_t)blocks * 11;     /* start of the current block */
    return base + rem_map[r];
}
