/*
 * A266255  Decimal representation of the n-th iteration of the "Rule 11"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(odd n)=2*4^n-4; a(even n>0)=3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    if ((n & (idx_t)1) == 0) {   /* even n > 0 */
        return (seq_t)3;
    }
    /* odd n */
    idx_t exp = 2 * n + 1;
    seq_t val = pow2(exp);
    return val - (seq_t)4;
}
