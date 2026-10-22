/*
 * A311611  Coordination sequence Gal.4.67.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for n > 0 let r = n mod 6 r = 0 : a(n) = 28 * (n / 6) r
 *            = 1 : a(n) = 28 * ((n - 1) / 6) + 4 r = 3 : a(n) = 28 * ((n - 3)
 *            / 6) + 12 r = 5 : a(n) = 28 * ((n - 5) / 6) + 22 r = 4 : a(n) =
 *            24 * ((n - 4) / 6) + 18 r = 2 : a(2) = 8; otherwise a(n) = 4 * n
 *            + 2
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

    idx_t r = n % 6;
    switch (r)
    {
    case 0:
        return (seq_t)28 * (n / 6);
    case 1:
        return (seq_t)28 * ((n - 1) / 6) + (seq_t)4;
    case 3:
        return (seq_t)28 * ((n - 3) / 6) + (seq_t)12;
    case 5:
        return (seq_t)28 * ((n - 5) / 6) + (seq_t)22;
    case 4:
        return (seq_t)24 * ((n - 4) / 6) + (seq_t)18;
    case 2:
        return (n == 2) ? (seq_t)8 : ((seq_t)4 * n + (seq_t)2);
    }
    /* unreachable, but keep the compiler happy */
    return (seq_t)0;
}
