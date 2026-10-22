/*
 * A315726  Coordination sequence Gal.6.252.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Coordination sequence Gal.6.252.6 (OEIS A315726)
   Sequence definition:
     a(0) = 1
     a(1) = 6
     a(2) = 12
     a(3) = 18
     For n ≥ 4:
        let k = n – 3
        let full = k / 10
        let rem  = k % 10
        a(n) = 1 + 17 + full·52 + (sum of the first rem terms of the cycle [4,4,4,4,6,6,6,6,6,6])
*/

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 6;
    if (n == 2) return 12;
    if (n == 3) return 18;

    /* cycle of differences from a(4) onwards */
    static const seq_t cycle[10] = {
        4, 4, 4, 4,   /* 4 fours  */
        6, 6, 6, 6, 6, 6   /* 6 sixes  */
    };

    seq_t sum = 17;                       /* sum of diff(1)..diff(3) = 5+6+6 */
    idx_t k = n - 3;            /* number of differences from diff(4) onwards */
    idx_t full = k / 10;
    idx_t rem  = k % 10;

    sum += (seq_t)full * 52;   /* 52 = 4*4 + 6*6 per cycle */

    for (idx_t i = 0; i < rem; ++i)
    {
        sum += cycle[i];
    }

    return 1 + sum;
}
