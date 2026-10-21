/*
 * A265225  Total number of ON (black) cells after n iterations of the "Rule
 *          54" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A265225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2*n - (-1)^n + 5)/4
 * Range:     u8 n <= 20, u16 n <= 360, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t T;
    if ((n & 1u) == 0) {
        T = (nn << 1u) + 4;   /* 2*n + 4  if n is even */
    } else {
        T = (nn << 1u) + 6;   /* 2*n + 6  if n is odd  */
    }
    return ((nn + 1) * T) / 4;
}
