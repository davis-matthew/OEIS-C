/*
 * A362738  a(n) is the least nonnegative integer solution for y such that x >
 *          1 is an integer in the equation n^y*x^n = n^(x^(1/n)).
 * https://oeis.org/A362738
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n - n^3 for n > 2; a(2)=a(3)=0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 20 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n <= 3) {
        return 0;
    }
    seq_t base = (seq_t)n;
    seq_t pow_n = ipow_idx(base, n);   /* n^n   */
    seq_t pow3  = ipow_idx(base, (idx_t)3); /* n^3   */
    return pow_n - pow3;
}
