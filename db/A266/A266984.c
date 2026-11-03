/*
 * A266984  Decimal representation of the n-th iteration of the "Rule 81"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((-4)^n+(-1)^n-1)/2+4^n for n>0, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 1000 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* 4^n */
    seq_t pow4 = ipow_i32((seq_t)4, n);

    /* Even n => a(n) = (3/2) * 4^n
       Odd  n => a(n) = (4^n)/2 - 1  */
    if ((n & (idx_t)1) == 0) {
        return (pow4 * (seq_t)3) / (seq_t)2;
    } else {
        return (pow4 / (seq_t)2) - (seq_t)1;
    }
}
