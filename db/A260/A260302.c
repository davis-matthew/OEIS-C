/*
 * A260302  Maximum water retention of a number octagon of order n.
 * https://oeis.org/A260302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n>2) ? ((7*n^2 - 18*n + 12)*(7*n^2 - 18*n + 13))/2 : 0
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=0, a(2)=0 */
seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t t1 = (seq_t)7 * n2 - (seq_t)18 * ns + (seq_t)12;
    seq_t t2 = (seq_t)7 * n2 - (seq_t)18 * ns + (seq_t)13;
    return (t1 * t2) / (seq_t)2;
}
