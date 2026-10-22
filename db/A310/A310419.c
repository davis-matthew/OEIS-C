/*
 * A310419  Coordination sequence Gal.5.130.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, if n even, a(n)=5*n; if n odd, a(n)=5*n-1 when
 *            n mod 8 ∈ {1,3}, otherwise a(n)=5*n+1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t base = (seq_t)n * 5;
    if ((n & 1) == 0)
        return base;
    idx_t mod8 = n & 7;
    if (mod8 == 1 || mod8 == 3)
        return base - 1;
    return base + 1;
}
