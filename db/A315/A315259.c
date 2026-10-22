/*
 * A315259  Coordination sequence Gal.4.54.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0: a(n)=5*n + 1 if n mod 8=1, a(n)=5*n-1 if n mod
 *            8=7, otherwise a(n)=5*n
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t five_n = (seq_t)n * 5;
    idx_t mod8 = n & 7u;
    if (mod8 == 1u) return five_n + 1;
    if (mod8 == 7u) return five_n - 1;
    return five_n;
}
