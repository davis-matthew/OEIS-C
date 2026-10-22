/*
 * A313090  Coordination sequence Gal.5.114.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=4; a(n)=a(n-1)+5 for the next 3 steps, +4 for the
 *            next 2 steps, repeating the pattern [5,5,5,4,4] (period 5)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t result = 4;          /* a(1) */
    idx_t cycle = 0;           /* 0,1,2,3,4 cyclically */
    for (idx_t i = 1; i < n; ++i) {
        if (cycle < 3)
            result += 5;
        else
            result += 4;
        cycle = (cycle + 1) % 5;
    }
    return result;
}
