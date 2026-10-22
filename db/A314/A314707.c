/*
 * A314707  Coordination sequence Gal.6.129.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 1 + |{ i ≤ n : i ∈ S }| where S = { 4, 8, 11, 12,
 *            15, 19, 22, 23, 26, 30, 33, 34, 37, … } S is generated from 4 by
 *            repeatedly adding gaps [4,3,1,3,4,3,1,3] in cyclic order
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base growth term: 4*n + 1 (when no extra increments) */
    seq_t result = ((seq_t)n) * 4 + 1;

    /* Gaps between successive indices in S (length 8).  */
    static const idx_t gaps[8] = {4, 3, 1, 3, 4, 3, 1, 3};

    idx_t pos = 4;          /* First element of S */
    int i = 0;              /* Index into gaps array (0..7) */
    idx_t cnt = 0;          /* Number of elements of S ≤ n */

    /* Count how many extra increments up to index n */
    while (pos <= n) {
        ++cnt;
        pos += gaps[i];
        i = (i + 1) & 7;     /* wrap around modulo 8 */
    }

    return result + cnt;
}
