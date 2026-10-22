/*
 * A374359  a(1) = 2, a(n) = 5 for n > 1.
 * https://oeis.org/A374359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr, cons, easy, less
 * Formula:   a(1)=2; a(n)=5 for n>1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 1) ? (seq_t)2 : (seq_t)5;
}
