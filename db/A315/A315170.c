/*
 * A315170  Coordination sequence Gal.6.119.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n >= 1, a(n) = 4*n + 2 + 2*floor(n/11) +
 *            2*floor((n-1)/11)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
    {
        return (seq_t)1;
    }

    /* compute 4*n + 2 in seq_t to avoid overflow in idx_t */
    seq_t part = (seq_t)4 * (seq_t)n + 2;

    /* floor divisions are performed in idx_t */
    idx_t q1 = n / 11;
    idx_t q2 = (n - 1) / 11;

    seq_t extra = (seq_t)2 * ((seq_t)(q1 + q2));

    return part + extra;
}
