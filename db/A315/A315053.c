/*
 * A315053  Coordination sequence Gal.6.345.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + d(n) where d(n) = { -1 if n≡2 or 5 (mod 12), +1 if
 *            n≡7 or 10 (mod 12), 0 otherwise } a(0) = 1
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
    seq_t base = (seq_t)5 * n;
    idx_t mod = n % 12;
    if (mod == 2 || mod == 5)
        return base - 1;
    if (mod == 7 || mod == 10)
        return base + 1;
    return base;
}
