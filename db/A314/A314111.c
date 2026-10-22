/*
 * A314111  Coordination sequence Gal.5.289.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 5*n + count_{1 <= m <= n, m mod 10
 *            == 2 or 9}. The count can be computed as 2 * floor(n/10) + [n
 *            mod 10 >= 2] + [n mod 10 >= 9]. This gives the OEIS sequence
 *            A314111
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

    idx_t cycles = n / 10;
    idx_t rem    = n % 10;

    /* number of extra increments from occurrences of 2 or 9 in each interval [1..n] */
    seq_t extra = (seq_t)(2 * cycles);
    if (rem >= 2)
        extra += 1;
    if (rem >= 9)
        extra += 1;

    /* 5 * n, cast n to seq_t first */
    return (seq_t)(5 * (seq_t)n) + extra;
}
