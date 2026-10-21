/*
 * A285193  Expansion of 1/(1+x+2*x^2) mod 3.
 * https://oeis.org/A285193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) for n>7, with a(0)=1, a(1)=2, a(2)=2, a(3)=0,
 *            a(4)=2, a(5)=1, a(6)=1, a(7)=0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t table[8] = {1, 2, 2, 0, 2, 1, 1, 0};
    return table[n & 7];
}
