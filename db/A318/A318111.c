/*
 * A318111  Number of maximal 1-intersecting families of 2-sets of [n] =
 *          {1,2,...,n}.
 * https://oeis.org/A318111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n,3) + n for n>3, a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t t = nn * (nn * nn - (seq_t)3 * nn + (seq_t)8);
    return t / (seq_t)6;
}
