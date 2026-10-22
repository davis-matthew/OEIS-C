/*
 * A312684  Coordination sequence Gal.6.252.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For n > 0, a(n)=6n‑2‑2*count4(n) where count4(n)=#{
 *            i∈[2,n] : i%10∈{0,1,2,9} }. This reproduces the diffusion of
 *            differences 3,4,6,… with a period 10 after the first term
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t nn   = (seq_t)n;
    seq_t res  = 6 * nn - (seq_t)2;   /* 6n – 2 */

    idx_t count4 = 0;
    for (idx_t i = 2; i <= n; ++i) {
        idx_t r = i % 10;
        if (r == 1 || r == 2 || r == 9 || r == 0)
            ++count4;
    }

    res -= 2 * (seq_t)count4;         /* subtract 2*count4 */
    return res;
}
