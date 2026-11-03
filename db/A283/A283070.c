/*
 * A283070  Sierpinski tetrahedron or tetrix numbers: a(n) = 2*4^n + 2.
 * https://oeis.org/A283070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*4^n + 2 (offset 0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 744 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t term = ipow_u64_idx(4, n);  /* compute 4^n   */
    return term * 2 + 2;          /* 2*4^n + 2     */
}
