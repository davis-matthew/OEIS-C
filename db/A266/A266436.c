/*
 * A266436  Decimal representation of the n-th iteration of the "Rule 23"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=0 if n is even and n>0; a(n)=2*4^n-1 if n is odd
 *            and n>0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    if ((n & 1) == 0) {   /* n is even */
        return 0;
    }
    /* n is odd and n>0 */
    seq_t val = pow4(n);     /* 4^n */
    val = val * 2;           /* 2*4^n */
    val = val - 1;           /* 2*4^n - 1 */
    return val;
}
