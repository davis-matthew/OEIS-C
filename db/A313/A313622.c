/*
 * A313622  Coordination sequence Gal.6.203.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n for n ≥ 1, with a(0)=1. For odd n ≡ 3 (mod 6), a(n) =
 *            5*n ± 1, where the sign alternates every 6 steps
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

    /* Compute 5*n */
    seq_t res = (seq_t)5 * (seq_t)n;

    /* Adjust for odd n ≡ 3 (mod 6) */
    if ((n & 1) && (n % 6 == 3)) {
        idx_t t = (n - 3) / 6;          /* t = 0,1,2,… for successive corrections */
        if ((t & 1) == 0)
            res -= (seq_t)1;           /* subtract 1 for even t (0,2,4,…) */
        else
            res += (seq_t)1;           /* add 1 for odd t (1,3,5,…) */
    }

    return res;
}
