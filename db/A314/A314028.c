/*
 * A314028  Coordination sequence Gal.6.198.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + (n%12==2 ? 1 : n%12==10 ? -1 : 0), a(0)=1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
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

    /* Compute 5*n safely in the result type */
    seq_t base = (seq_t)n * (seq_t)5;
    idx_t m = n % (idx_t)12;

    if (m == (idx_t)2)
        return base + (seq_t)1;
    if (m == (idx_t)10)
        return base - (seq_t)1;

    return base;
}
