/*
 * A315630  Coordination sequence Gal.6.233.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6; for n ≥ 2, a(n)=a(n-1)+6 if n%10∈{0,1,2,9},
 *            otherwise a(n)=a(n-1)+4
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    seq_t result = (seq_t)6;      /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        /* Use the period of 10 to decide the increment. */
        if (i % 10 == 0 || i % 10 == 1 || i % 10 == 2 || i % 10 == 9)
            result += (seq_t)6;
        else
            result += (seq_t)4;
    }
    return result;
}
