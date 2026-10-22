/*
 * A315641  Coordination sequence Gal.6.208.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0: a(n) = floor(16*n/3) + d[n % 12] where d =
 *            {0, 1, 2, 0, 0, 0, 0, 1, 1, 0, -1, 0}
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0)
        return (seq_t)1;

    /* Periodic correction array */
    static const int d[12] = {0, 1, 2, 0, 0, 0, 0, 1, 1, 0, -1, 0};

    /* Base term: floor(16*n/3) */
    seq_t base = ((seq_t)n * (seq_t)16) / (seq_t)3;

    /* Apply the periodic correction, careful with negative values */
    int delta = d[n % 12];
    if (delta >= 0)
        base += (seq_t)delta;
    else
        base -= (seq_t)(-delta);

    return base;
}
