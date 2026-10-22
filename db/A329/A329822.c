/*
 * A329822  The minimum weight of a Boolean function of algebraic degree at
 *          most n-3 whose support contains n linearly independent elements.
 * https://oeis.org/A329822
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n for n>=6; with a(3)=8, a(4)=8, a(5)=12
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)8;
    if (n == 5) return (seq_t)12;
    return (seq_t)n * (seq_t)2;
}
