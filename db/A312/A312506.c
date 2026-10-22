/*
 * A312506  Coordination sequence Gal.5.109.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + 2 + 4*⌊(n-3)/8⌋ + ( ((n-3) mod 8) >= 3 ? 2 : 0 )
 *            with a(0)=1, a(1)=4, a(2)=8. The sequence is linear except for
 *            small adjustments every few terms as described above
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
    if (n == 1 || n == 2)
        return (seq_t)4 * (seq_t)n;

    idx_t m   = n - 3;
    idx_t quo = m / 8;   /* floor((n-3)/8) */
    idx_t rem = m % 8;   /* (n-3) mod 8 */

    seq_t offset = (seq_t)2 + (seq_t)4 * (seq_t)quo + ((rem < 3) ? (seq_t)0 : (seq_t)2);
    return (seq_t)4 * (seq_t)n + offset;
}
