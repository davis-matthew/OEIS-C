/*
 * A314254  Coordination sequence Gal.6.628.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 6*n - (2*(n/12) + (n%12?1:0))
 * Range:     u8 n <= 43, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t tn = (seq_t)n;
    seq_t q  = (seq_t)(n / 12);
    seq_t r  = (seq_t)(n % 12);
    seq_t c  = 2 * q + (r ? 1 : 0);
    return 6 * tn - c;
}
