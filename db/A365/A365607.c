/*
 * A365607  Number of degree 3 vertices in the n-Sierpinski carpet graph.
 * https://oeis.org/A365607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 35*a(n-2) + 24*a(n-3) for n>3 with initial
 *            values a(1)=0, a(2)=40, a(3)=328
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)40;
    if (n == 3) return (seq_t)328;

    seq_t a_minus3 = (seq_t)0;      /* a(1) */
    seq_t a_minus2 = (seq_t)40;     /* a(2) */
    seq_t a_minus1 = (seq_t)328;    /* a(3) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        cur = (seq_t)12 * a_minus1 - (seq_t)35 * a_minus2 + (seq_t)24 * a_minus3;
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = cur;
    }
    return cur;
}
