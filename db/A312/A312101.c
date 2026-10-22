/*
 * A312101  Coordination sequence Gal.6.150.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((9*n+1)/2) + c[n mod 12], with c =
 *            {0,-1,-1,-1,0,-1,0,0,0,0,1,0} Special case: a(0) = 1
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

    /* base = floor((9*n + 1)/2) */
    seq_t base = ((seq_t)n * 9 + 1) / 2;

    /* periodic adjustment */
    static const int8_t adjust[12] = {
        0, -1, -1, -1, 0, -1, 0, 0, 0, 0, 1, 0
    };
    int8_t corr = adjust[(int)(n % 12)];

    if (corr < 0)
        return base - (seq_t)(-corr);
    else
        return base + (seq_t)corr;
}
