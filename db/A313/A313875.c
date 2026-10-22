/*
 * A313875  Coordination sequence Gal.4.134.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + floor((n+5)/8) + floor((n+2)/8) + (n==0?1:0)
 * Range:     u8 n <= 48, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 5*n */
    seq_t res = 5 * (seq_t)n;

    /* add the part that depends on the position of n */
    if (n == 0) {
        res += 1;                   /* a(0) = 1 */
    }

    /* count of indices k in [1,n] with k≡3 or 6 (mod 8), i.e. where
       the difference a(k)-a(k-1) = 6 */
    idx_t d6 = (n + 5) / 8 + (n + 2) / 8;

    res += (seq_t)d6;               /* add the 6‑increments */
    return res;                     /* result is a(n) */
}
