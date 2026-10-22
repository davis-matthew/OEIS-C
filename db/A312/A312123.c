/*
 * A312123  Coordination sequence Gal.6.120.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9*n + s(n)) / 2 where s(0)=2, s(12k)=0 for k>0, and for
 *            n>0 with n%12=r (1≤r≤11) s(r) is given by the table below: r : 1
 *            2 3 4 5 6 7 8 9 10 11 s : -1 -2 -1 0 1 0 -1 0 1 2 1
 * Range:     u8 n <= 28, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int offs[12] = { 2, -1, -2, -1, 0, 1, 0, -1, 0, 1, 2, 1 };
    idx_t rem = n % 12;
    int s = offs[rem];
    if (n != 0 && rem == 0)
        s = 0;                  /* multiples of 12 except n=0 */

    seq_t prod = (seq_t)n * 9;   /* 9*n */

    if (s >= 0)
        return (prod + (seq_t)s) / 2;
    else
        return (prod - (seq_t)(-s)) / 2;
}
