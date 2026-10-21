/*
 * A266607  Decimal representation of the n-th iteration of the "Rule 39"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 2^k a(2k+1) = 2^(4k+3) - (2^k + 1) (k ≥ 0)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if ((n & 1) == 0) {
        return pow2(n >> 1);
    } else {
        idx_t k = n >> 1;          /* k = (n-1)/2 */
        return pow2((k << 2) + 3) - (pow2(k) + (seq_t)1);
    }
}
