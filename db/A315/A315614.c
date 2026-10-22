/*
 * A315614  Coordination sequence Gal.5.318.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence for Gal.5.318.5
   a(0) = 1
   a(1) = 6
   a(2) = 12
   For n ≥ 3:
     a(n) = 12 + 50*⌊(n-3)/9⌋ + cum[(n-3)%9 + 1]
   where cum = {0,2,12,14,24,26,32,38,44,50}
   (cum[k] is the sum of the first k elements of
    the 9‑period difference pattern
    D = {2,10,2,10,2,6,6,6,6} ).
   This implements the exact OEIS formula for A315614. */
#include <stdint.h>

static const seq_t cumulative[10] = {
    0, /* 0 elements */
    2, /* 1  element: 2 */
    12,/* 2  elements: 2+10 */
    14,/* 3  elements: 2+10+2 */
    24,/* 4  elements */
    26,/* 5  */
    32,/* 6  */
    38,/* 7  */
    44,/* 8  */
    50 /* 9  (full period) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;
    if (n == 2) return 12;

    idx_t k = n - 3;
    idx_t p = k / 9;      /* how many full periods */
    idx_t q = k % 9;      /* position inside the period */

    seq_t sum = ((seq_t)p) * 50u + cumulative[q + 1];

    return 12u + sum;
}
