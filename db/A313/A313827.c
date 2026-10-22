/*
 * A313827  Coordination sequence Gal.5.141.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + add 1 if n mod 10 == 3, subtract 1 if n mod 10 ==
 *            7, otherwise add 0. a(0) = 1
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                /* base case */
        return (seq_t)1;

    seq_t val = (seq_t)n * (seq_t)5;
    idx_t r = n % (idx_t)10;

    if (r == (idx_t)3)
        val += (seq_t)1;
    else if (r == (idx_t)7)
        val -= (seq_t)1;

    return val;
}
